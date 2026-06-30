#include <iostream>
#include <vector>
#include "Net.h"
#include "NetworkVisualizer.h"

int main()
{
	// Define network topology: 5 layers with 5 neurons each
	std::vector<size_t> topology = { 5, 5, 5, 5, 5 };
	Net myNet(topology);
	NetworkVisualizer visualizer(myNet);

	std::cout << "--- Spiking Neural Network Simulation ---\n";
	std::cout << "Press and hold keys 1 to 5 to inject manual spikes!\n";

	// Main simulation loop
	while (visualizer.isOpen())
	{
		// 1. Process user input
		visualizer.handleEvents(myNet);

		// 2. Step the network physics
		myNet.update();

		// 3. Render visualization
		visualizer.update();
		visualizer.render();
	}

	return 0;
}