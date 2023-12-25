#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
private:
    int _speed;
    int _direction;
    sf::Sprite _sprite;

public:
    bullet(int direction, float x, float y, sf::Texture& texture, int speed)
        : _direction(direction), _speed(speed)
    {
        _sprite.setTexture(_texture);
        _sprite.setPosition(x, y);
    }

    void move()
    {
        sf::Vector2f coords = _sprite.getPosition();
        if (_direction == 0) {
            coords.x += _speed;
        }
        if (_direction == 1) {
            coords.y -= _speed;
        }
        if (_direction == 2) {
            coords.x -= _speed;
        }
        if (_direction == 3) {
            coords.y += _speed;
        }
        _sprite.setPosition(coords);
    }

    operator sf::Sprite()
    {
        return _sprite;
    }
};