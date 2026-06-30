#pragma once
#include <SFML/Graphics.hpp>
#include "Net.h"

class NetworkVisualizer
{
public:
	// Read-only reference to ensure the visualizer does not modify network state
	NetworkVisualizer(const Net& a_net);

	void update();
	void render();
	bool isOpen() const { return window.isOpen(); }

	// Mutable reference required to inject external stimuli (EPSPs) via keyboard
	void handleEvents(Net& a_net);

private:
	const Net& net;
	sf::RenderWindow window;
	sf::Font font;

	// Layout configuration
	const float radius = 15.0f;
	const float xSpacing = 150.0f;
	const float ySpacing = 80.0f;
	const float offsetX = 100.0f;
	const float offsetY = 100.0f;

	// Translates logical network topology indices to 2D screen coordinates
	sf::Vector2f getScreenPosition(size_t layer, size_t index) const;
};