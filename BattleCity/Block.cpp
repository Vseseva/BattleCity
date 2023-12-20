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

bool Block::isOnBlock(float x, float y)
{
	if (!_isSolid)
		return true;
	sf::Vector2f pos = _sprite.getPosition();
	sf::FloatRect bounds = _sprite.getLocalBounds();
	return pos.x <= x && x <= (pos.x + bounds.width) && pos.y <= y && y <= (pos.y + bounds.height);
}