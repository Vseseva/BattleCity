#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Game
{
public:
	Game();

	void run();
private:
	int _width;
	int _height;
	sf::RenderWindow _window;
};
