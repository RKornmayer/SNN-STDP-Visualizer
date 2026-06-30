#pragma once

#include <vector>
#include <memory>

#include "Neuron.h"
#include "Synapse.h"
#include "SmallDatatypes.h"

class Net
{
public:
	// Initializes the network topology and wires all feedforward and collateral synapses
	Net(const std::vector<size_t>& a_topology);

	// Injects external stimuli into the first layer
	void setInput(const std::vector<short>& a_input);

	// Advances the network physics by one time step
	void update();

	// Accessors for rendering and state observation
	const std::vector<size_t>& getTopology() const { return topology; }
	const std::vector<std::vector<Neuron>>& getLayers() const { return layers; }
	const std::vector<std::unique_ptr<Synapse>>& getSynapses() const { return synapses; }

private:
	std::vector<size_t> topology;
	std::vector<std::vector<Neuron>> layers;
	std::vector<std::unique_ptr<Synapse>> synapses;

	// Global constant for lateral blanket inhibition
	const double lateralInhibition = -2.0;
};