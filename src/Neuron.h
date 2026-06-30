#pragma once

#include <vector>
#include "SmallDatatypes.h"

class Synapse;

class Neuron
{
public:
	Neuron(position a_netPos);

	// Topology setup
	void addIncomingSynapse(Synapse* synapse);
	void addOutgoingSynapse(Synapse* synapse);

	// Core physics and lifecycle
	void fire();
	void updateActivity();
	void learn();
	void normalizeWeights();

	void addEPSP(double a_epsp) { potential += a_epsp; }

	// State accessors
	double getPotential() const { return potential; }
	short getActivity() const { return activity; }
	void setActivity(short a_activity) { activity = a_activity; }

	double getThreshold() const { return threshold; }
	void setThreshold(double a_threshold) { threshold = a_threshold; }

	position getNPosition() const { return net_position; }

private:
	// O(1) routing via direct memory references
	std::vector<Synapse*> incomingSynapses;
	std::vector<Synapse*> outgoingSynapses;

	double potential;
	double threshold = 20.0;

	// Dynamic threshold scaling factors
	double refractoryThresholdIncrease = 10.0;
	double refractoryThresholdDecrease = 10.0;

	// State machine: 0 = resting, 1 = firing, 2 = refractory
	short activity = 0;
	double magnitude = 0.0;

	position net_position;
};