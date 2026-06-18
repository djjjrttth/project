#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Map {
private:
    // Двумерная матрица лабиринта (вектор строк)
    // '#' - стена, '.' - монетка, ' ' - пустота
    std::vector<std::string> grid; 
    
    float tileSize; // Размер одной клетки лабиринта в пикселях (например, 40.f)

    // Текстуры или простые шейпы для отрисовки
    sf::RectangleShape wallShape;
    sf::CircleShape coinShape;

public:
    // Конструктор: задает начальные размеры и цвета
    Map();

    // Шаг 2.2: Этот метод вызовет Александра (feature/level), 
    // чтобы передать тебе сетку, прочитанную из файла level1.txt
    void loadLevel(const std::vector<std::string>& newGrid);

    // Шаг 2.1: Функция проверки коллизий со стенами
    // Принимает координаты игрока в пикселях и возвращает true, если там СТЕНА
    bool isWall(float pixelX, float pixelY, float playerWidth, float playerHeight);

    // Шаг 2.3: Обработка сбора монеток
    // Проверяет, наступил ли игрок на монетку. Если да — удаляет её и увеличивает счетчик
    void checkCoinCollision(float playerX, float playerY, float playerWidth, float playerHeight, int& score);

    // Метод отрисовки карты в окне игры
    void draw(sf::RenderWindow& window);
};