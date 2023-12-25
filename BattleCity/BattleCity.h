﻿#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Button.h"
#include "Block.h"
#include "Tank.hpp"

class Game
{
public:
	Game();

	void run();
	int menu(sf::Sprite& bgSprite, const int frameNum, const float animationDuration, sf::Time& elapsedTime, sf::Clock& clock, sf::Sprite& cursor);
	int options(sf::Sprite& cursor);
	int start();
	int levelStart(int level, int& score);
	void statistics(int score);
private:
	int _width;
	int _height;
	std::vector<sf::Text> _texts;
	std::vector<Button> _buttons;
	sf::RenderWindow _window;

};
