#pragma once
#include <cstddef>

// Represents the 2D coordinate of a neuron within the network
struct position
{
	size_t lx; // Layer index
	size_t ny; // Neuron index within the layer
};