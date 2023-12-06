#include "BattleCity.h"

Game::Game()
{
	_height = 720;
	_width = 1280;
}

void Game::run()
{

    // Создание фона
    sf::Texture bg;
    bg.loadFromFile("images/bg-menu.png");
    sf::Sprite bgSprite;
    bgSprite.setTexture(bg);
    bgSprite.setTextureRect(sf::IntRect(0, 0, _width, _height));
    bgSprite.setPosition(0, 0);

    // Настройки для создания анимированного фона
    const int frameNum = 29;
    const float animationDuration = 5;

    // Создание окна
	_window.create(sf::VideoMode(_width, _height), "Battle city");

    // Переменные для контроля изменения фона
    sf::Time elapsedTime;
    sf::Clock clock;

    // Основной цикл программы
    while (_window.isOpen())
    {
        sf::Event event;
        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                _window.close();
        }

        sf::Time deltaTime = clock.restart();
        elapsedTime += deltaTime;
        float timeAsSecond = elapsedTime.asSeconds();

        int animFrame = static_cast<int>((timeAsSecond / animationDuration) * static_cast<float>(frameNum)) % frameNum;

        bgSprite.setTextureRect(sf::IntRect(0, animFrame * _height, _width, _height));

        _window.clear(sf::Color::Black);
        _window.draw(bgSprite);
        _window.display();
    }
}
