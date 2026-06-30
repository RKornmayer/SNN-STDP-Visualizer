#pragma once

class Neuron;

class Synapse
{
public:
	Synapse(Neuron* src, Neuron* tgt, double initialWeight, bool plastic = true);

	// Core synaptic functions
	void transmit();
	void applyPlasticity();

	double getWeight() const { return weight; }
	void setWeight(double a_weight) { weight = a_weight; }

	Neuron* getSource() const { return source; }
	Neuron* getTarget() const { return target; }

private:
	// O(1) routing via direct memory references
	Neuron* source;
	Neuron* target;

	double weight;
	bool isPlastic;
};