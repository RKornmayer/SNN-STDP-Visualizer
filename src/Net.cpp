#include "Net.h"
#include <random>
#include <algorithm>

Net::Net(const std::vector<size_t>& a_topology)
	: topology(a_topology)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	// Initialize with gentle excitatory weights to prevent early network explosions
	std::uniform_real_distribution<double> dist(0.05, 0.3);

	// 1. Create neurons per layer
	for (size_t l = 0; l < topology.size(); ++l)
	{
		layers.push_back(std::vector<Neuron>());
		layers[l].reserve(topology[l]);

		for (size_t n = 0; n < topology[l]; ++n)
		{
			layers[l].push_back(Neuron({ l, n }));
		}
	}

	// 2. Wire the network
	for (size_t l = 0; l < layers.size(); ++l)
	{
		for (size_t n = 0; n < layers[l].size(); ++n)
		{
			Neuron* sourceNeuron = &layers[l][n];

			// --- FEEDFORWARD: Excitatory & Plastic ---
			if (l < layers.size() - 1)
			{
				for (size_t target_n = 0; target_n < layers[l + 1].size(); ++target_n)
				{
					Neuron* targetNeuron = &layers[l + 1][target_n];
					double initialWeight = dist(gen);

					synapses.push_back(std::make_unique<Synapse>(sourceNeuron, targetNeuron, initialWeight, true));
					Synapse* synPtr = synapses.back().get();

					sourceNeuron->addOutgoingSynapse(synPtr);
					targetNeuron->addIncomingSynapse(synPtr);
				}
			}

			// --- COLLATERAL: Blanket Inhibition & Static ---
			for (size_t target_n = 0; target_n < layers[l].size(); ++target_n)
			{
				if (n != target_n)
				{
					Neuron* targetNeuron = &layers[l][target_n];

					synapses.push_back(std::make_unique<Synapse>(sourceNeuron, targetNeuron, lateralInhibition, false));
					Synapse* synPtr = synapses.back().get();

					sourceNeuron->addOutgoingSynapse(synPtr);
					targetNeuron->addIncomingSynapse(synPtr);
				}
			}
		}
	}
}

void Net::setInput(const std::vector<short>& a_input)
{
	for (size_t n = 0; n < a_input.size(); ++n)
	{
		if (a_input[n] == 1)
		{
			// Only inject external current if the neuron is in a resting state
			if (layers[0][n].getActivity() == 0)
			{
				layers[0][n].addEPSP(21.0);
			}
		}
	}
}

void Net::update()
{
	// PHASE 1: Local State Update (Calculate potentials and dynamic thresholds)
	for (auto& layer : layers)
	{
		for (auto& neuron : layer)
		{
			neuron.updateActivity();

			if (neuron.getActivity() == 1)
			{
				neuron.setThreshold(neuron.getThreshold() * 1.5);
			}
			else
			{
				// Decay threshold, clamped at 0.1 to maintain baseline sensitivity
				neuron.setThreshold(std::max(0.1, neuron.getThreshold() * 0.9));
			}
		}
	}

	// PHASE 2: Network Communication (Spike transmission and STDP evaluation)
	for (auto& layer : layers)
	{
		for (auto& neuron : layer)
		{
			if (neuron.getActivity() == 1)
			{
				neuron.fire();
			}

			// Evaluate STDP rules (requires checking post-synaptic states like refractory)
			neuron.learn();
		}
	}

	// PHASE 3: Homeostasis (L1 normalization of incoming excitatory weights)
	for (auto& layer : layers)
	{
		for (auto& neuron : layer)
		{
			neuron.normalizeWeights();
		}
	}
}