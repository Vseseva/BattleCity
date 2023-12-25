#include "BattleCity.h"
#include <string>
#include <fstream>
#include <iostream>

Game::Game()
{
	_height = 720;
	_width = 1280;
    _buttons.clear();
    _window.create(sf::VideoMode(_width, _height), "Battle city");

    // Добавление иконки для окна
    sf::Image icon;
    icon.loadFromFile("images/icon.png");
    _window.setIcon(32, 32, icon.getPixelsPtr());
    _window.setVerticalSyncEnabled(true);
    _window.setFramerateLimit(60);
}

void Game::run()
{
    // Инициализация шрифта для кнопок
    sf::Font* font = new sf::Font();
    (*font).loadFromFile("fonts/font.otf");

    // Инициализация шрифта для меню настроек
    sf::Font* arial = new sf::Font();
    (*arial).loadFromFile("fonts/arial.ttf");

    // Создание фона
    sf::Texture bg;
    bg.loadFromFile("images/bg-menu.png");
    sf::Sprite bgSprite;
    bgSprite.setTexture(bg);
    bgSprite.setTextureRect(sf::IntRect(0, 0, 1280, 720));
    bgSprite.setPosition(0, 0);

    // Настройки для создания анимированного фона
    const int frameNum = 29;
    const float animationDuration = 4;

    // Переменные для контроля изменения фона
    sf::Time elapsedTime;
    sf::Clock clock;

    // Делаем обычный курсор невидимым
    _window.setMouseCursorVisible(false);

    // Создание курсора-танка
    sf::Texture cursorTexture;
    cursorTexture.loadFromFile("images/cursorRed.png");
    sf::Sprite cursor(cursorTexture);
    cursor.setRotation(-45);

    // Основной цикл программы
    int button = 0;
    while (button != -1)
    {
        // Добавление кнопок меню
        _buttons = { Button(550, 190, 40, L"Новая игра", font), Button(560, 270, 40, L"Настройки", font), Button(615, 350, 40, L"Выйти", font) };
        button = menu(bgSprite, frameNum, animationDuration, elapsedTime, clock, cursor);
        if (button == 0)
        {
            _buttons = { Button(575, 190, 40, L"1 Игрок", font), Button(560, 270, 40, L"2 Игрока", font) };
            int players = menu(bgSprite, frameNum, animationDuration, elapsedTime, clock, cursor);
            if (players == -1)
            {
                button = -1;
                continue;
            }
            button = start();
        }
        else if (button == 1)
        {
            _texts = { sf::Text(), sf::Text(), sf::Text(), sf::Text()};
            std::vector<sf::String> strings = { L"Игрок 1", L"Выстрел", L"Игрок 2", L"Выстрел" };
            for (int i = 0; i < 4; ++i)
            {
                _texts[i].setFont(*font);
                _texts[i].setString(strings[i]);
                _texts[i].setCharacterSize(42);
                _texts[i].setFillColor(sf::Color::Black);
                _texts[i].setStyle(sf::Text::Bold);
                _texts[i].setPosition(100 + (_width-100) / 4 * i, 500);
            }
            _buttons = { Button(175, 340, 40, "W", arial), Button(100, 410, 40, "A", arial), Button(175, 410, 40, "S", arial), Button(250, 410, 40, "D", arial), Button(500, 410, 40, "H", arial),
            Button(765, 340, 40, L"↑", arial), Button(690, 410, 40, L"←", arial), Button(765, 410, 40, L"↓", arial), Button(840, 410, 40, L"→", arial), Button(1090, 410, 40, "2", arial),
            Button(30, 30, 40, L"Назад", font)};
            button = options(cursor);
            _texts = {};
        }
        else if (button == 2)
            button = -1;
    }
    _window.close();
}

int Game::menu(sf::Sprite& bgSprite, const int frameNum, const float animationDuration, sf::Time& elapsedTime, sf::Clock& clock, sf::Sprite& cursor)
{
    while (_window.isOpen())
    {
        // Получение позиции курсора
        float cursorX = sf::Mouse::getPosition(_window).x - 7.5;
        float cursorY = sf::Mouse::getPosition(_window).y + 15;

        // Обработка событий
        sf::Event event;
        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                return -1;
            else if (event.type == sf::Event::Resized)
            {
                _window.setSize(sf::Vector2u(event.size.width, event.size.height));
                _width = event.size.width;
                _height = event.size.height;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                for (int i = 0; i < _buttons.size(); ++i)
                    if (_buttons[i].isOnButton(cursorX, cursorY))
                    {
                        std::string text = ((sf::Text)_buttons[i]).getString();
                        return i;
                    }
            }
        }


        sf::Time deltaTime = clock.restart();
        elapsedTime += deltaTime;
        float timeAsSecond = elapsedTime.asSeconds();

        // Вычисление текущего кадра
        int animFrame = static_cast<int>((timeAsSecond / animationDuration) * static_cast<float>(frameNum)) % frameNum;
        bgSprite.setTextureRect(sf::IntRect(0, animFrame * 720, 1280, 720));

        // Перемещение спрайта курсора на позицию курсора мыши
        cursor.setPosition(sf::Vector2f(cursorX, cursorY));

        // Подсветка кнопок
        for (int i = 0; i < _buttons.size(); ++i)
        {
            if (_buttons[i].isOnButton(cursorX, cursorY))
                _buttons[i].setColor(sf::Color::Green);
            else
                _buttons[i].setColor(sf::Color::Red);
        }

        // Отрисовка объектов
        _window.clear(sf::Color::Black);
        _window.draw(bgSprite);
        for (int i = 0; i < _buttons.size(); ++i)
            _window.draw(_buttons[i]);
        _window.draw(cursor);
        _window.display();
    }
}

int Game::options(sf::Sprite& cursor)
{
    while (_window.isOpen())
    {
        // Получение позиции курсора
        float cursorX = sf::Mouse::getPosition(_window).x - 7.5;
        float cursorY = sf::Mouse::getPosition(_window).y + 15;

        // Обработка событий
        sf::Event event;
        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                return -1;
            else if (event.type == sf::Event::Resized)
            {
                _window.setSize(sf::Vector2u(event.size.width, event.size.height));
                _width = event.size.width;
                _height = event.size.height;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                for (int i = 0; i < _buttons.size(); ++i)
                    if (_buttons[i].isOnButton(cursorX, cursorY))
                    {
                        std::string text = ((sf::Text)_buttons[i]).getString();
                        return i;
                    }
            }
        }

        // Перемещение спрайта курсора на позицию курсора мыши
        cursor.setPosition((sf::Vector2f)(sf::Mouse::getPosition(_window)));

        // Подсветка кнопок
        for (int i = 0; i < _buttons.size(); ++i)
        {
            if (_buttons[i].isOnButton(cursorX, cursorY))
                _buttons[i].setColor(sf::Color::Red);
            else
                _buttons[i].setColor(sf::Color::Green);
        }

        // Отрисовка объектов
        _window.clear(sf::Color(104, 131, 174));
        for (int i = 0; i < _buttons.size(); ++i)
            _window.draw(_buttons[i]);
        for (int i = 0; i < _texts.size(); ++i)
            _window.draw(_texts[i]);
        _window.draw(cursor);
        _window.display();
    }
}

int Game::start()
{
    int score = 0;
    for (int i = 1; i < 6; ++i)
    {
        int code = levelStart(i, score);
        if (code == -1)
            return -1;
    }
    statistics(score);
    return 0;
}

int Game::levelStart(int level, int& score)
{
    sf::Font* arial = new sf::Font();
    (*arial).loadFromFile("fonts/arial.ttf");
    std::vector<Block> objects;
    objects.reserve(40 * 21);
    std::string filename = "levels/level" + std::to_string(level) + ".txt";
    std::ifstream file(filename);
    sf::Texture solid, road, p1;
    solid.loadFromFile("images/destructibleBlock0st.png");
    road.loadFromFile("images/road.png");
    p1.loadFromFile("images/t34S.png");
    Tank player1(3, 3, 0, 0, p1);

    int code = 0;
    for (int i = 0; i < 21; ++i)
    {
        for (int j = 0; j < 40; ++j)
        {
            file >> code;
            if (code == 1)
                objects.push_back(Block(true, solid, j * 32, 48 + i * 32));
            else if (code == 0)
                objects.push_back(Block(false, road, j * 32, 48 + i * 32));
            else if (code == 2)
            {
                objects.push_back(Block(false, road, j * 32, 48 + i * 32));
                player1.Move(j * 32 / player1.GetSpeed(), (48 + i * 32) / player1.GetSpeed());
            }
        }
    }

    _texts.push_back(sf::Text());

    std::vector<sf::String> strings = { L"HP: " + sf::String(std::to_string(player1.GetHp()))};
    for (int i = 0; i < 1; ++i)
    {
        _texts[i].setFont(*arial);
        _texts[i].setString(strings[i]);
        _texts[i].setCharacterSize(30);
        _texts[i].setFillColor(sf::Color::White);
        _texts[i].setStyle(sf::Text::Bold);
        _texts[i].setPosition(100 + (_width - 100) / 4 * i, 0);
    }

    while (_window.isOpen())
    {
        // Обработка событий
        sf::Event event;
        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                return -1;
            else if (event.type == sf::Event::Resized)
            {
                _window.setSize(sf::Vector2u(event.size.width, event.size.height));
                _width = event.size.width;
                _height = event.size.height;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                bool flag = true;
                sf::Text text(sf::String(L"Игра приостановлена"), *arial, 30);
                text.setColor(sf::Color::Green);
                text.setPosition(_width / 2 - 150, _height / 2);
                sf::RectangleShape rect(sf::Vector2f(_width, _height));
                sf::Color color(200, 100, 0, 128);
                rect.setFillColor(color);

                _window.draw(rect);
                _window.draw(text);
                _window.display();
                while (flag)
                {
                    while (_window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                            return -1;
                        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                            flag = false;
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::W && !objects[(int)(player1.GetPositionY() - player1.GetSpeed() - 48) / 32 * 40 + (int)player1.GetPositionX() / 32].isSolid() && !objects[(int)(player1.GetPositionY() - player1.GetSpeed() - 48) / 32 * 40 + (int)player1.GetPositionX() / 32 + 1].isSolid())
                    player1.Move(0, -1);
                else if (event.key.code == sf::Keyboard::S && !objects[(int)(player1.GetPositionY() + player1.GetSpeed() - 16) / 32 * 40 + (int)player1.GetPositionX() / 32].isSolid() && !objects[(int)(player1.GetPositionY() + player1.GetSpeed() - 16) / 32 * 40 + (int)player1.GetPositionX() / 32 + 1].isSolid())
                    player1.Move(0, 1);
                else if (event.key.code == sf::Keyboard::A && !objects[(int)(player1.GetPositionY() - 48) / 32 * 40 + (int)(player1.GetPositionX() - player1.GetSpeed()) / 32].isSolid() && !objects[(int)(player1.GetPositionY() - 16) / 32 * 40 + (int)(player1.GetPositionX() - player1.GetSpeed()) / 32].isSolid())
                    player1.Move(-1, 0);
                else if (event.key.code == sf::Keyboard::D && !objects[(int)(player1.GetPositionY() - 48) / 32 * 40 + (int)(player1.GetPositionX() + player1.GetSpeed()) / 32 + 1].isSolid() && !objects[(int)(player1.GetPositionY() - 16) / 32 * 40 + (int)(player1.GetPositionX() + player1.GetSpeed()) / 32 + 1].isSolid())
                    player1.Move(1, 0);
            }
        }

        // Отрисовка объектов
        _window.clear();
        for (int i = 0; i < objects.size(); ++i)
            _window.draw(objects[i]);
        for (int i = 0; i < _texts.size(); ++i)
            _window.draw(_texts[i]);
        _window.draw(player1);
        _window.display();
    }
    return 0;
}

void Game::statistics(int score)
{}