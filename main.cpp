#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>

int main() {
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tomb of the Mask - Map Test");
    window.setFramerateLimit(60); // Ограничим FPS, чтобы точка не летала слишком быстро

    Map gameMap;

    // Твой тестовый лабиринт
    std::vector<std::string> testLevel = {
        "##########",
        "#........#",
        "#.####.#.#",
        "#.#    #.#",
        "#......#.#",
        "##########"
    };
    gameMap.loadLevel(testLevel);

    // ВРЕМЕННЫЙ ИГРОК ДЛЯ ТЕСТА ТВОЕЙ КАРТЫ
    sf::CircleShape player(12.f); // Радиус 12 пикселей (диаметр 24)
    player.setFillColor(sf::Color::Red); // Пусть пока будет красной
    
    // Начальная позиция (внутри лабиринта, клетка 1,1 -> x=45, y=45)
    float playerX = 45.f;
    float playerY = 45.f;
    player.setPosition(playerX, playerY);

    float playerSpeed = 3.f; // Скорость движения точки
    int score = 0;           // Твой счетчик очков для монеток

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Логика движения: вычисляем, куда игрок ХОЧЕТ пойти
        float nextX = playerX;
        float nextY = playerY;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  nextX -= playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) nextX += playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    nextY -= playerSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  nextY += playerSpeed;

        // Размеры хитбокса нашей точки (24 на 24 пикселя)
        float pWidth = 24.f;
        float pHeight = 24.f;

        // ШАГ 2.1: Твоя проверка коллизий! 
        // Если впереди НЕТ стены — разрешаем передвинуть точку
        if (!gameMap.isWall(nextX, nextY, pWidth, pHeight)) {
            playerX = nextX;
            playerY = nextY;
            player.setPosition(playerX, playerY);
        }

        // ШАГ 2.3: Твоя проверка сбора монеток!
        gameMap.checkCoinCollision(playerX, playerY, pWidth, pHeight, score);

        // Отрисовка кадра
        window.clear();
        
        gameMap.draw(window); // Рисуем твою карту и монетки
        window.draw(player);  // Рисуем игрока поверх карты
        
        window.display();
    }

    return 0;
}