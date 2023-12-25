#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Button
{
public:
	Button(int x, int y, int characterSize, std::string text, sf::Font* font);
	bool isOnButton(float x, float y);
	void setColor(sf::Color color);
	operator sf::Text()
	{
		return _text;
	}
private:
	int _posX;
	int _posY;
	sf::Text _text;
};