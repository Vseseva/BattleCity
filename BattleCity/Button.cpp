#include "Button.h"
#include <iostream>

Button::Button(int x, int y, int characterSize, std::string text, sf::Font* font)
{
	_posX = x;
	_posY = y;
	_text.setFont(*font);
	_text.setString(text);
	_text.setCharacterSize(characterSize);
	_text.setFillColor(sf::Color::Black);
	_text.setStyle(sf::Text::Bold);
	_text.setPosition(_posX, _posY);
}

bool Button::isOnButton(float x, float y)
{
	return _posX <= x && x <= (_posX + _text.getLocalBounds().width) && _posY <= y && y <= (_posY + _text.getLocalBounds().height);
}

void Button::setColor(sf::Color color)
{
	_text.setColor(color);
}