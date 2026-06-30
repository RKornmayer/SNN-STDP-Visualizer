#include "NetworkVisualizer.h"
#include <iostream>
#include <cstdint>

NetworkVisualizer::NetworkVisualizer(const Net& a_net)
	: net(a_net), window(sf::VideoMode({ 800, 600 }), "SNN Real-Time Visualizer")
{
	// Cap framerate to stabilize the simulation speed and prevent CPU overloading
	window.setFramerateLimit(60);
}

sf::Vector2f NetworkVisualizer::getScreenPosition(size_t layer, size_t index) const
{
	return sf::Vector2f(
		offsetX + static_cast<float>(layer) * xSpacing,
		offsetY + static_cast<float>(index) * ySpacing
	);
}

void NetworkVisualizer::handleEvents(Net& a_net)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
	}

	// Map numerical keys to the first layer for manual spike injection
	std::vector<short> currentInput(a_net.getTopology()[0], 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) currentInput[0] = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) && currentInput.size() > 1) currentInput[1] = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3) && currentInput.size() > 2) currentInput[2] = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4) && currentInput.size() > 3) currentInput[3] = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5) && currentInput.size() > 4) currentInput[4] = 1;

	// Dispatch input if any valid key is pressed
	for (short val : currentInput) {
		if (val == 1) {
			a_net.setInput(currentInput);
			break;
		}
	}
}

void NetworkVisualizer::update()
{
	// Placeholder for future UI animations, camera logic, or visual transitions
}

void NetworkVisualizer::render()
{
	window.clear(sf::Color(30, 30, 35));

	// 1. Render synapses first to keep them in the background (z-ordering)
	for (const auto& syn : net.getSynapses())
	{
		position srcPos = syn->getSource()->getNPosition();
		position tgtPos = syn->getTarget()->getNPosition();

		sf::Vector2f p1 = getScreenPosition(srcPos.lx, srcPos.ny);
		sf::Vector2f p2 = getScreenPosition(tgtPos.lx, tgtPos.ny);

		// Center the lines on the somas
		p1.x += radius; p1.y += radius;
		p2.x += radius; p2.y += radius;

		// Map synaptic weight to grayscale intensity
		uint8_t colorIntensity = static_cast<uint8_t>(std::min(255.0, std::max(50.0, syn->getWeight() * 255.0)));
		sf::Color synColor(colorIntensity, colorIntensity, colorIntensity, 150);

		sf::Vertex line[2];
		line[0].position = p1;
		line[0].color = synColor;
		line[1].position = p2;
		line[1].color = synColor;

		window.draw(line, 2, sf::PrimitiveType::Lines);
	}

	// 2. Render neuron somas
	const auto& layers = net.getLayers();
	for (size_t l = 0; l < layers.size(); ++l)
	{
		for (size_t n = 0; n < layers[l].size(); ++n)
		{
			sf::CircleShape soma(radius);
			soma.setPosition(getScreenPosition(l, n));
			soma.setOutlineThickness(2.0f);
			soma.setOutlineColor(sf::Color(100, 100, 100));

			// Color coding based on local activity state
			short activity = layers[l][n].getActivity();
			if (activity == 0) soma.setFillColor(sf::Color(50, 50, 50));       // Resting
			else if (activity == 1) soma.setFillColor(sf::Color(250, 250, 250)); // Firing
			else if (activity == 2) soma.setFillColor(sf::Color(150, 0, 80));    // Refractory

			window.draw(soma);
		}
	}

	window.display();
}