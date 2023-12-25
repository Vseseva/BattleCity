#include "Block.h"

Block::Block(bool isSolid, sf::Texture& texture, float x, float y)
{
	_isSolid = isSolid;
	_sprite = sf::Sprite(texture);
	_sprite.setPosition(x, y);
}

bool Block::isSolid()
{
	return _isSolid;
}

sf::Vector2f Block::getPosition()
{
	return _sprite.getPosition();
}

sf::FloatRect Block::getGlobalBounds()
{
	return _sprite.getGlobalBounds();
}