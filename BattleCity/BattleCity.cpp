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
    bgSprite.setTextureRect(sf::IntRect(0, 0, 960, 540));
    bgSprite.setPosition(0, 0);
    bgSprite.setScale(4. / 3, 4. / 3);

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
            button = start(players + 1);
            _texts = {};
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
            _buttons = { Button(175, 340, 40, "W", arial), Button(100, 410, 40, "A", arial), Button(175, 410, 40, "S", arial), Button(250, 410, 40, "D", arial), Button(500, 410, 40, "Space", arial),
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
        if (_window.pollEvent(event))
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
        bgSprite.setTextureRect(sf::IntRect(0, animFrame * 540, 960, 540));

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
        if (_window.pollEvent(event))
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

int Game::start(int players)
{
    int score = 0;
    int hp = players == 1 ? 3 : 5;
    for (int i = 1; i < 6; ++i)
    {
        int code = levelStart(i, players, score, hp);
        _texts = {};
        if (code == -1)
            return -1;
        else if (code == 1)
            break;
    }
    return statistics(score);
}

int Game::levelStart(int level, int players, int& score, int& hp)
{
    sf::Font* arial = new sf::Font();
    (*arial).loadFromFile("fonts/arial.ttf");
    std::vector<Block> objects;
    objects.reserve(40 * 21);
    std::string filename = "levels/level" + std::to_string(level) + ".txt";
    std::ifstream file(filename);
    sf::Texture solid, road, p1, p2, bulletFriend, enemy, bulletEnemy, health, shield;
    solid.loadFromFile("images/destructibleBlock0st.png");
    road.loadFromFile("images/road.png");
    p1.loadFromFile("images/t34S.png");
    p2.loadFromFile("images/p2.png");
    bulletFriend.loadFromFile("images/bullet1.png");
    enemy.loadFromFile("images/tiger1S.png");
    bulletEnemy.loadFromFile("images/bullet2.png");
    health.loadFromFile("images/heal.png");
    shield.loadFromFile("images/shield.png");

    Tank player1(hp, 4, 0, 0, p1);
    Tank player2(0, 4, 0, 0, p2);
    std::vector<Bullet> bulletsFriends;

    std::vector<Tank> enemies;
    std::vector<Bullet> bulletsEnemies;

    std::vector<int> enemiesSpawn;

    std::vector<Bonus> bonuses;

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
            else if (code == 3)
            {
                objects.push_back(Block(false, road, j * 32, 48 + i * 32));
                enemiesSpawn.push_back(i*40 + j);
            }
            else if (code == 4)
            {
                objects.push_back(Block(false, road, j * 32, 48 + i * 32));
                if (players == 2)
                    player2.Move(j * 32 / player2.GetSpeed(), (48 + i * 32) / player2.GetSpeed());
            }
        }
    }

    int enemiesCount = 0;
    file >> enemiesCount;
    if (players == 2)
        enemiesCount *= 1.5;

    _texts = { sf::Text(), sf::Text(), sf::Text() };

    std::vector<sf::String> strings = { L"HP: " + sf::String(std::to_string(player1.GetHp())), L"Enemies: " + sf::String(std::to_string(enemiesCount)), L"Score: " + sf::String(std::to_string(score)) };
    for (int i = 0; i < strings.size(); ++i)
    {
        _texts[i].setFont(*arial);
        _texts[i].setString(strings[i]);
        _texts[i].setCharacterSize(30);
        _texts[i].setFillColor(sf::Color::White);
        _texts[i].setStyle(sf::Text::Bold);
        _texts[i].setPosition(100 + (_width - 100) / 4 * i, 0);
    }

    sf::Clock clock;
    sf::Clock playerClock;
    sf::Clock shootClock;

    sf::Time deltaTime = clock.restart();
    sf::Time deltaPlayer = playerClock.restart();
    sf::Time deltaShoot = shootClock.restart();

    float shootSec = deltaShoot.asSeconds() + 0.5;
    float playerSec = deltaPlayer.asSeconds() + 0.5;
    float timeAsSecond = deltaTime.asSeconds() + 3;

    while (_window.isOpen())
    {
        // Обработка событий
        sf::Event event;
        if (_window.pollEvent(event))
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
                    while (_window.waitEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                            return -1;
                        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                            flag = false;
                    }
                }
            }
        }

        // Обработка нажатий клавиш
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            bool flag = !objects[(int)(player1.GetPositionY() - player1.GetSpeed() - 48) / 32 * 40 + (int)player1.GetPositionX() / 32].isSolid() && !objects[(int)(player1.GetPositionY() - player1.GetSpeed() - 48) / 32 * 40 + (int)player1.GetPositionX() / 32 + 1].isSolid();
            if (flag)
            {
                sf::FloatRect rect = player1.getGlobalBounds();
                rect.top -= player1.GetSpeed();
                for (int i = 0; i < enemies.size(); ++i)
                    if (rect.intersects(enemies[i].getGlobalBounds()))
                    {
                        flag = false;
                        player1.Move(0, 1.5);
                        player1.Move(0, -0.5);
                        break;
                    }
            }
            if (flag)
                player1.Move(0, -1);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            bool flag = !objects[(int)(player1.GetPositionY() + player1.GetSpeed() - 16) / 32 * 40 + (int)player1.GetPositionX() / 32].isSolid() && !objects[(int)(player1.GetPositionY() + player1.GetSpeed() - 16) / 32 * 40 + (int)player1.GetPositionX() / 32 + 1].isSolid();
            if (flag)
            {
                sf::FloatRect rect = player1.getGlobalBounds();
                rect.top += player1.GetSpeed();
                for (int i = 0; i < enemies.size(); ++i)
                    if (rect.intersects(enemies[i].getGlobalBounds()))
                    {
                        flag = false;
                        player1.Move(0, -1.5);
                        player1.Move(0, 0.5);
                        break;
                    }
            }
            if (flag)
                player1.Move(0, 1);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            bool flag = !objects[(int)(player1.GetPositionY() - 48) / 32 * 40 + (int)(player1.GetPositionX() - player1.GetSpeed()) / 32].isSolid() && !objects[(int)(player1.GetPositionY() - 16) / 32 * 40 + (int)(player1.GetPositionX() - player1.GetSpeed()) / 32].isSolid();
            if (flag)
            {
                sf::FloatRect rect = player1.getGlobalBounds();
                rect.left -= player1.GetSpeed();
                for (int i = 0; i < enemies.size(); ++i)
                    if (rect.intersects(enemies[i].getGlobalBounds()))
                    {
                        flag = false;
                        player1.Move(1.5, 0);
                        player1.Move(-0.5, 0);
                        break;
                    }
            }
            if (flag)
                player1.Move(-1, 0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            bool flag = !objects[(int)(player1.GetPositionY() - 48) / 32 * 40 + (int)(player1.GetPositionX() + player1.GetSpeed()) / 32 + 1].isSolid() && !objects[(int)(player1.GetPositionY() - 16) / 32 * 40 + (int)(player1.GetPositionX() + player1.GetSpeed()) / 32 + 1].isSolid();
            if (flag)
            {
                sf::FloatRect rect = player1.getGlobalBounds();
                rect.left += player1.GetSpeed();
                for (int i = 0; i < enemies.size(); ++i)
                    if (rect.intersects(enemies[i].getGlobalBounds()))
                    {
                        flag = false;
                        player1.Move(-1.5, 0);
                        player1.Move(0.5, 0);
                        break;
                    }
            }
            if (flag)
                player1.Move(1, 0);
        }
        if (playerSec > 0.4 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bulletsFriends.size() < 7) {
            bulletsFriends.push_back(player1.Shoot(bulletFriend));
            deltaPlayer = playerClock.restart();
        }
        if (players == 2)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                bool flag = !objects[(int)(player2.GetPositionY() - player2.GetSpeed() - 48) / 32 * 40 + (int)player2.GetPositionX() / 32].isSolid() && !objects[(int)(player2.GetPositionY() - player2.GetSpeed() - 48) / 32 * 40 + (int)player2.GetPositionX() / 32 + 1].isSolid();
                if (flag)
                {
                    sf::FloatRect rect = player2.getGlobalBounds();
                    rect.top -= player2.GetSpeed();
                    for (int i = 0; i < enemies.size(); ++i)
                        if (rect.intersects(enemies[i].getGlobalBounds()))
                        {
                            flag = false;
                            player1.Move(0, 1.5);
                            player1.Move(0, -0.5);
                            break;
                        }
                }
                if (flag)
                    player2.Move(0, -1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                bool flag = !objects[(int)(player2.GetPositionY() + player2.GetSpeed() - 16) / 32 * 40 + (int)player2.GetPositionX() / 32].isSolid() && !objects[(int)(player2.GetPositionY() + player2.GetSpeed() - 16) / 32 * 40 + (int)player2.GetPositionX() / 32 + 1].isSolid();
                if (flag)
                {
                    sf::FloatRect rect = player2.getGlobalBounds();
                    rect.top += player2.GetSpeed();
                    for (int i = 0; i < enemies.size(); ++i)
                        if (rect.intersects(enemies[i].getGlobalBounds()))
                        {
                            flag = false;
                            player2.Move(0, -1.5);
                            player2.Move(0, 0.5);
                            break;
                        }
                }
                if (flag)
                    player2.Move(0, 1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                bool flag = !objects[(int)(player2.GetPositionY() - 48) / 32 * 40 + (int)(player2.GetPositionX() - player2.GetSpeed()) / 32].isSolid() && !objects[(int)(player2.GetPositionY() - 16) / 32 * 40 + (int)(player2.GetPositionX() - player2.GetSpeed()) / 32].isSolid();
                if (flag)
                {
                    sf::FloatRect rect = player2.getGlobalBounds();
                    rect.left -= player2.GetSpeed();
                    for (int i = 0; i < enemies.size(); ++i)
                        if (rect.intersects(enemies[i].getGlobalBounds()))
                        {
                            flag = false;
                            player2.Move(1.5, 0);
                            player2.Move(-0.5, 0);
                            break;
                        }
                }
                if (flag)
                    player2.Move(-1, 0);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                bool flag = !objects[(int)(player2.GetPositionY() - 48) / 32 * 40 + (int)(player2.GetPositionX() + player2.GetSpeed()) / 32 + 1].isSolid() && !objects[(int)(player2.GetPositionY() - 16) / 32 * 40 + (int)(player2.GetPositionX() + player2.GetSpeed()) / 32 + 1].isSolid();
                if (flag)
                {
                    sf::FloatRect rect = player2.getGlobalBounds();
                    rect.left += player2.GetSpeed();
                    for (int i = 0; i < enemies.size(); ++i)
                        if (rect.intersects(enemies[i].getGlobalBounds()))
                        {
                            flag = false;
                            player2.Move(-1.5, 0);
                            player2.Move(0.5, 0);
                            break;
                        }
                }
                if (flag)
                    player2.Move(1, 0);
            }
            if (playerSec > 0.4 && (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) && bulletsFriends.size() < 7)
            {
                bulletsFriends.push_back(player2.Shoot(bulletFriend));
                deltaPlayer = playerClock.restart();
            }
        }

        // Изменение содержимого текстов
        std::vector<sf::String> strings = { L"HP: " + sf::String(std::to_string(player1.GetHp())), L"Enemies: " + sf::String(std::to_string(enemiesCount)), L"Score: " + sf::String(std::to_string(score))};
        for (int i = 0; i < strings.size(); ++i)
            _texts[i].setString(strings[i]);

        // Спавн врагов
        if (enemies.size() < 5 && enemiesCount != enemies.size() && timeAsSecond > 2) {
            int num = rand() % enemiesSpawn.size();
            bool flag = true;
            for (int i = 0; i < enemies.size(); ++i)
                flag = flag && !enemies[i].getGlobalBounds().intersects(objects[enemiesSpawn[num]].getGlobalBounds());
            flag = flag && !player1.getGlobalBounds().intersects(objects[enemiesSpawn[num]].getGlobalBounds());
            if (flag)
                enemies.push_back(Tank(1, 2, objects[enemiesSpawn[num]].getPosition().x, objects[enemiesSpawn[num]].getPosition().y, enemy));

            deltaTime = clock.restart();
        }

        // Обработка столкновения пуль с врагами и стенами
        for (int i = 0; i < bulletsFriends.size(); ++i)
        {
            bulletsFriends[i].move();
            sf::Vector2f coords = bulletsFriends[i].getPosition();
            if (objects[(int)coords.x / 32 + (int)(coords.y - 48) / 32 * 40].isSolid() || objects[(int)(coords.x + 16) / 32 + (int)(coords.y - 32) / 32 * 40].isSolid())
                bulletsFriends.erase(bulletsFriends.begin() + i);
            else {
                for (int j = 0; j < enemies.size(); ++j) {
                    if (bulletsFriends[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
                        bulletsFriends.erase(bulletsFriends.begin() + i);
                        enemies[j].TakeDamage();
                        break;
                    }
                }
            }
        }

        // Действия бонусов
        for (int i = 0; i < bonuses.size(); ++i)
        {
            if (player1.getGlobalBounds().intersects(bonuses[i].getGlobalBounds()) || (players == 2 && player2.getGlobalBounds().intersects(bonuses[i].getGlobalBounds())))
            {
                int type = bonuses[i].getType();
                if (type == 0 && (player1.GetHp() < 3 || players == 2 && player1.GetHp() < 5))
                    player1.SetHp(players == 1 ? 3 : 5);
                else if (type == 1)
                    player1.SetHp(player1.GetHp() + 1);
                bonuses.erase(bonuses.begin() + i);
                break;
            }
        }

        // Действия врагов
        sf::Vector2f playerCoords(player1.GetPositionX(), player1.GetPositionY());
        sf::Vector2f player2Coords(player2.GetPositionX(), player2.GetPositionY());
        for (int i = 0; i < enemies.size(); ++i) {
            sf::Vector2f tankCoords(enemies[i].GetPositionX(), enemies[i].GetPositionY());
            int direction = enemies[i].GetDirection();
            if (shootSec > 0.8 && bulletsEnemies.size() < 7 && (
                direction == 0 && playerCoords.x > tankCoords.x && abs(playerCoords.y - tankCoords.y) < 28 ||
                direction == 1 && playerCoords.y > tankCoords.y && abs(playerCoords.x - tankCoords.x) < 28 ||
                direction == 2 && playerCoords.x < tankCoords.x && abs(playerCoords.y - tankCoords.y) < 28 ||
                direction == 3 && playerCoords.y < tankCoords.y && abs(playerCoords.x - tankCoords.x) < 28 ||
                players == 2 && (
                direction == 0 && player2Coords.x > tankCoords.x && abs(player2Coords.y - tankCoords.y) < 28 ||
                direction == 1 && player2Coords.y > tankCoords.y && abs(player2Coords.x - tankCoords.x) < 28 ||
                direction == 2 && player2Coords.x < tankCoords.x && abs(player2Coords.y - tankCoords.y) < 28 ||
                direction == 3 && player2Coords.y < tankCoords.y && abs(player2Coords.x - tankCoords.x) < 28 )
                )
                )
            {
                bulletsEnemies.push_back(enemies[i].Shoot(bulletEnemy));
                deltaShoot = shootClock.restart();
            }
            else if (abs(playerCoords.y - tankCoords.y) < 28)
            {
                int direction = playerCoords.x > tankCoords.x ? 0 : 2;
                enemies[i].SetDirection(direction);
            }
            else if (abs(playerCoords.x - tankCoords.x) < 28)
            {
                int direction = playerCoords.y > tankCoords.y ? 1 : 3;
                enemies[i].SetDirection(direction);
            }
            else if (players == 2 && abs(player2Coords.y - tankCoords.y) < 28)
            {
                int direction = player2Coords.x > tankCoords.x ? 0 : 2;
                enemies[i].SetDirection(direction);
            }
            else if (players == 2 && abs(player2Coords.x - tankCoords.x) < 28)
            {
                int direction = player2Coords.y > tankCoords.y ? 1 : 3;
                enemies[i].SetDirection(direction);
            }
            else if (direction == 0 && !objects[(int)(tankCoords.y - 48) / 32 * 40 + (int)(tankCoords.x + enemies[i].GetSpeed()) / 32 + 1].isSolid() && !objects[(int)(tankCoords.y - 16) / 32 * 40 + (int)(tankCoords.x + enemies[i].GetSpeed()) / 32 + 1].isSolid())
            {
                bool flag = true;
                sf::FloatRect rect = enemies[i].getGlobalBounds();
                rect.left += enemies[i].GetSpeed();
                for (int j = 0; j < enemies.size(); ++j)
                    if (i != j && rect.intersects(enemies[j].getGlobalBounds()))
                    {
                        flag = false;
                        if (!objects[(int)(tankCoords.y - 48) / 32 * 40 + (int)(tankCoords.x - enemies[i].GetSpeed()) / 32].isSolid() && !objects[(int)(tankCoords.y - 16) / 32 * 40 + (int)(tankCoords.x - enemies[i].GetSpeed()) / 32].isSolid())
                        {
                            enemies[i].Move(-1.5, 0);
                            enemies[i].Move(0.5, 0);
                        }
                        break;
                    }
                if (flag)
                    enemies[i].Move(1, 0);
            }
            else if (direction == 3 && !objects[(int)(tankCoords.y - enemies[i].GetSpeed() - 48) / 32 * 40 + (int)(tankCoords.x) / 32].isSolid() && !objects[(int)(tankCoords.y - enemies[i].GetSpeed() - 48) / 32 * 40 + (int)tankCoords.x / 32 + 1].isSolid())
            {
                bool flag = true;
                sf::FloatRect rect = enemies[i].getGlobalBounds();
                rect.left += enemies[i].GetSpeed();
                for (int j = 0; j < enemies.size(); ++j)
                    if (i != j && rect.intersects(enemies[j].getGlobalBounds()))
                    {
                        flag = false;
                        if (!objects[(int)(tankCoords.y + enemies[i].GetSpeed() - 16) / 32 * 40 + (int)(tankCoords.x) / 32].isSolid() && !objects[(int)(tankCoords.y + enemies[i].GetSpeed() - 16) / 32 * 40 + (int)tankCoords.x / 32 + 1].isSolid())
                        {
                            enemies[i].Move(0, 1.5);
                            enemies[i].Move(0, -0.5);
                        }
                        break;
                    }
                if (flag)
                    enemies[i].Move(0, -1);
            }
            else if (direction == 2 && !objects[(int)(tankCoords.y - 48) / 32 * 40 + (int)(tankCoords.x - enemies[i].GetSpeed()) / 32].isSolid() && !objects[(int)(tankCoords.y - 16) / 32 * 40 + (int)(tankCoords.x - enemies[i].GetSpeed()) / 32].isSolid())
            {
                bool flag = true;
                sf::FloatRect rect = enemies[i].getGlobalBounds();
                rect.left += enemies[i].GetSpeed();
                for (int j = 0; j < enemies.size(); ++j)
                    if (i != j && rect.intersects(enemies[j].getGlobalBounds()))
                    {
                        flag = false;
                        if (!objects[(int)(tankCoords.y - 48) / 32 * 40 + (int)(tankCoords.x + enemies[i].GetSpeed()) / 32 + 1].isSolid() && !objects[(int)(tankCoords.y - 16) / 32 * 40 + (int)(tankCoords.x + enemies[i].GetSpeed()) / 32 + 1].isSolid())
                        {
                            enemies[i].Move(1.5, 0);
                            enemies[i].Move(-0.5, 0);
                        }
                        break;
                    }
                if (flag)
                    enemies[i].Move(-1, 0);
            }
            else if (direction == 1 && !objects[(int)(tankCoords.y + enemies[i].GetSpeed() - 16) / 32 * 40 + (int)(tankCoords.x) / 32].isSolid() && !objects[(int)(tankCoords.y + enemies[i].GetSpeed() - 16) / 32 * 40 + (int)tankCoords.x / 32 + 1].isSolid())
            {
                bool flag = true;
                sf::FloatRect rect = enemies[i].getGlobalBounds();
                rect.left += enemies[i].GetSpeed();
                for (int j = 0; j < enemies.size(); ++j)
                    if (i != j && rect.intersects(enemies[j].getGlobalBounds()))
                    {
                        flag = false;
                        if (!objects[(int)(tankCoords.y - enemies[i].GetSpeed() - 48) / 32 * 40 + (int)(tankCoords.x) / 32].isSolid() && !objects[(int)(tankCoords.y - enemies[i].GetSpeed() - 48) / 32 * 40 + (int)tankCoords.x / 32 + 1].isSolid())
                        {
                            enemies[i].Move(0, -1.5);
                            enemies[i].Move(0, 0.5);
                        }
                        break;
                    }
                if (flag)
                    enemies[i].Move(0, 1);
            }
            else
                enemies[i].SetDirection((direction + rand()) % 4);
            if (enemies[i].GetHp() == 0) {
                int chance = rand() % 8;
                if (chance == 0)
                    bonuses.push_back(Bonus(0, enemies[i].GetPositionX(), enemies[i].GetPositionY(), health));
                else if (chance == 1)
                    bonuses.push_back(Bonus(1, enemies[i].GetPositionX(), enemies[i].GetPositionY(), shield));
                enemies.erase(enemies.begin() + i);
                --enemiesCount;
                score += 100;
            }
        }

        // Обработка стокновения пуль с игроком и стенами
        for (int i = 0; i < bulletsEnemies.size(); ++i)
        {
            bulletsEnemies[i].move();
            sf::Vector2f coords = bulletsEnemies[i].getPosition();
            if (objects[(int)coords.x / 32 + (int)(coords.y - 48) / 32 * 40].isSolid() || objects[(int)(coords.x + 16) / 32 + (int)(coords.y - 32) / 32 * 40].isSolid())
                bulletsEnemies.erase(bulletsEnemies.begin() + i);
            else if (bulletsEnemies[i].getGlobalBounds().intersects(player1.getGlobalBounds()) || (players == 2 && bulletsEnemies[i].getGlobalBounds().intersects(player2.getGlobalBounds())))
            {
                bulletsEnemies.erase(bulletsEnemies.begin() + i);
                player1.TakeDamage();
                break;
            }
        }


        timeAsSecond = clock.getElapsedTime().asSeconds();
        playerSec = playerClock.getElapsedTime().asSeconds();
        shootSec = shootClock.getElapsedTime().asSeconds();

        // Отрисовка объектов
        _window.clear();
        for (int i = 0; i < objects.size(); ++i)
            _window.draw(objects[i]);
        for (int i = 0; i < _texts.size(); ++i)
            _window.draw(_texts[i]);
        for (int i = 0; i < bonuses.size(); ++i)
             _window.draw(bonuses[i]);
        for (int i = 0; i < bulletsEnemies.size(); ++i)
            _window.draw(bulletsEnemies[i]);
        for (int i = 0; i < enemies.size(); ++i)
            _window.draw(enemies[i]);
        for (int i = 0; i < bulletsFriends.size(); ++i)
            _window.draw(bulletsFriends[i]);
        _window.draw(player1);
        if (players == 2)
            _window.draw(player2);
        _window.display();
        if (player1.GetHp() == 0)
            return 1;
        else if (enemiesCount == 0)
        {
            hp = player1.GetHp();
            score += 500;
            break;
        }
    }
    return 0;
}

int Game::statistics(int score)
{
    sf::Font* font = new sf::Font();
    (*font).loadFromFile("fonts/font.otf");
    std::ifstream fileIn("scoreboard.txt");
    std::vector<int> scoreboard(10);
    for (int i = 0; i < 10; ++i)
        fileIn >> scoreboard[i];
    fileIn.close();
    scoreboard.push_back(score);

    std::sort(scoreboard.begin(), scoreboard.end(),[](int a, int b) { return a > b; });

    std::ofstream fileOut("scoreboard.txt");
    for (int i = 0; i < 10; ++i)
        fileOut << scoreboard[i] << std::endl;
    fileOut.close();

    _texts = { sf::Text(), sf::Text(), sf::Text(), sf::Text(), sf::Text(), sf::Text(), sf::Text(), sf::Text(), sf::Text(), sf::Text() };
    for (int i = 0; i < 10; ++i)
    {
        _texts[i].setFont(*font);
        _texts[i].setString(sf::String(std::to_string(i + 1)) + L". " + sf::String(std::to_string(scoreboard[i])) + L" pt");
        _texts[i].setCharacterSize(30);
        _texts[i].setFillColor(sf::Color::White);
        _texts[i].setStyle(sf::Text::Bold);
        _texts[i].setPosition(_width / 2 - 100, 100 + (_height - 200) / 10 * i);
    }

    _window.clear();
    for (int i = 0; i < _texts.size(); ++i)
        _window.draw(_texts[i]);
    _window.display();

    _sleep(2000);

    return 0;
}