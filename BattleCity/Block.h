#pragma once
#include <SFML/Graphics.hpp>

class Block
{
public:
	Block(bool isSolid, sf::Texture& texture, float x, float y);
	operator sf::Sprite()
	{
		return _sprite;
	}
	bool isSolid();
	bool isOnBlock(float x, float y);
private:
	bool _isSolid;
	sf::Sprite _sprite;
};