#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Tank
{
public:
	Tank(int hp, float speed, float positionX, float positionY, sf::Texture& texture)
	{
		this->_hp		  = hp;
		this->_speed	  = speed;
		this->_positionX = positionX;
		this->_positionY = positionY;
		this->_direction = 0;
		this->_sprite.setTexture(texture);
		this->_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		this->_sprite.setPosition(_positionX, _positionY);
	}

	int GetHp() { return this->_hp; }
	float GetSpeed() { return this->_speed; }
	float GetPositionX() { return this->_positionX; }
	float GetPositionY() { return this->_positionY; }
	int GetDirection() { return this->_direction; }
	void SetHp(int hp) { this->_hp = hp; }
	void SetSpeed(float speed) { this->_speed = speed; }
	void SetDirection(int direction) 
	{ 
		this->_direction = direction;
		if (direction == 0)
			_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		else if (direction == 1)
			_sprite.setTextureRect(sf::IntRect(96, 0, 32, 32));
		else if (direction == 2)
			_sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
		else if (direction == 3)
			_sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
	}

	Bullet Shoot(sf::Texture& texture) {
		return Bullet(_direction, _sprite.getPosition().x + 8, _sprite.getPosition().y + 8, texture, 5);
	}
	void Move(float offsetX, float offsetY)
	{
		this->_positionX += offsetX * this->_speed;
		this->_positionY += offsetY * this->_speed;
		if (offsetX > 0 && this->_direction != 0)
			SetDirection(0);
		else if (offsetX < 0 && this->_direction != 2)
			SetDirection(2);
		else if (offsetY > 0 && this->_direction != 1)
			SetDirection(1);
		else if (offsetY < 0 && this->_direction != 3)
			SetDirection(3);
		this->_sprite.setPosition(_positionX, _positionY);
	}
	void TakeDamage() { this->_hp -= 1; }
	operator sf::Sprite() { return _sprite; }
	sf::FloatRect getGlobalBounds() { return _sprite.getGlobalBounds(); }

private:
	int _hp;
	float _speed;
	int _direction;
	float _positionX;
	float _positionY;
	sf::Sprite _sprite;
};
