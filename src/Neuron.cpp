#include "Neuron.h"
#include "Synapse.h"
#include <cmath>
#include <random>


Neuron::Neuron(position a_netPos)
	: net_position(a_netPos), potential(0.0)
{
}

void Neuron::addIncomingSynapse(Synapse* synapse)
{
	incomingSynapses.push_back(synapse);
}

void Neuron::addOutgoingSynapse(Synapse* synapse)
{
	outgoingSynapses.push_back(synapse);
}


void Neuron::fire()
{
	// O(1) Routing: Trigger transmission on all outgoing connections
	for (Synapse* syn : outgoingSynapses)
	{
		syn->transmit();
	}
}


void Neuron::updateActivity()
{
	// STATIC: Instantiated once in memory to optimize CPU cycle cost
	static std::random_device rd;
	static std::mt19937 gen(rd());
	// Gaussian noise to simulate spontaneous background firing (mEPSPs)
	static std::normal_distribution<double> noiseDist(0.0, 0.035);

	// Inject thermodynamic noise into resting potential
	if (activity == 0)
	{
		potential += noiseDist(gen);
	}

	// State Machine: Resting (0) -> Firing (1) -> Refractory (2)
	if (activity == 0 && potential >= threshold)
	{
		activity = 1;
		potential = 0.0;
	}
	else if (activity == 1)
	{
		activity = 2;
		threshold *= refractoryThresholdIncrease;
	}
	else if (activity == 2)
	{
		activity = 0;
		threshold /= refractoryThresholdDecrease;
	}

	// Apply membrane leak
	if (potential != 0.0)
	{
		potential *= 0.9;
	}

	// Hard-cap hyperpolarization to simulate the biological Reversal Potential (e.g., -5.0).
	// Prevents lateral inhibition from pushing the neuron into an inescapable negative state.
	potential = std::max(-5.0, potential);
}


void Neuron::learn()
{
	// Delegate STDP evaluation to the synapse to enforce temporal causality rules
	for (Synapse* syn : incomingSynapses)
	{
		syn->applyPlasticity();
	}
}

void Neuron::normalizeWeights()
{
	magnitude = 0.0;
	for (Synapse* syn : incomingSynapses)
	{
		// Exclude inhibitory/static collateral synapses from normalization
		if (syn->getWeight() > 0.0)
		{
			// L1-Normalization (sum of absolutes) enforces sparsity and selective pathways
			magnitude += std::abs(syn->getWeight());
		}
	}

	for (Synapse* syn : incomingSynapses)
	{
		if (syn->getWeight() > 0.0 && magnitude > 0.0)
		{
			syn->setWeight(syn->getWeight() / magnitude);
		}
	}
}