#pragma once
#include <SFML/Graphics.hpp>

class Bonus
{
public:
	Bonus(int type, float x, float y, sf::Texture& texture)
	{
		this->_type = type;
		this->_positionX = x;
		this->_positionY = y;
		this->_sprite.setTexture(texture);
		this->_sprite.setPosition(x, y);
	}
	operator sf::Sprite()
	{
		return _sprite;
	}
	sf::FloatRect getGlobalBounds()
	{
		return _sprite.getGlobalBounds();
	}
	int getType() 
	{
		return _type;
	}
private:
	int _type;
	float _positionX;
	float _positionY;
	sf::Sprite _sprite;
};