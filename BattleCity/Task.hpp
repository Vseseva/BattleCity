#pragma once
#include"TaskApi.hpp"

class Tank
{
public:
	Tank(int hp, int speed, float possitionX, float possitionY)
	{
		this->_hp		  = hp;
		this->_speed	  = speed;
		this->_possitionX = possitionX;
		this->_possitionY = possitionY;
	}

	int GetHp() { return this->_hp; }
	int GetSpeed() { return this->_speed; }
	float GetPossitionX() { return this->_possitionX; }
	float GetPossitionY() { return this->_possitionY; }
	void SetHp(int hp) { this->_hp = hp; }
	void SetSpeed(int speed) { this->_speed = speed; }

	int Shoot();
	void Move(float offsetX, float offsetY)
	{
		this->_possitionX += offsetX * this->_speed;
		this->_possitionY += offsetY * this->_speed;
	}
	void TakeDamage() { this->_hp -= 1; }

private:
	int _hp;
	int _speed;
	float _possitionX;
	float _possitionY;
};
