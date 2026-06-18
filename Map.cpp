#include "Map.h"

Map::Map() {
    tileSize = 40.f; // Пусть одна клетка будет 40х40 пикселей

    // Настраиваем внешний вид стены (синий квадрат, как в Tomb of the Mask)
    wallShape.setSize(sf::Vector2f(tileSize, tileSize));
    wallShape.setFillColor(sf::Color(0, 0, 255)); 

    // Настраиваем внешний вид монетки (желтый кружок)
    coinShape.setRadius(6.f);
    coinShape.setFillColor(sf::Color(255, 215, 0));
    // Сдвиг, чтобы монетка была ровно по центру клетки 40х40
    coinShape.setOrigin(-14.f, -14.f); 
}

void Map::loadLevel(const std::vector<std::string>& newGrid) {
    grid = newGrid;
}

// === ШАГ 2.1: ЛОГИКА ПРОВЕРКИ КОЛЛИЗИЙ (isWall) ===
bool Map::isWall(float pixelX, float pixelY, float playerWidth, float playerHeight) {
    // Если карта еще не загружена, ходить нельзя
    if (grid.empty()) return true;

    // Проверяем все 4 угла хитбокса игрока, чтобы он не проваливался краями в стены
    float cornersX[4] = { pixelX, pixelX + playerWidth, pixelX, pixelX + playerWidth };
    float cornersY[4] = { pixelY, pixelY, pixelY + playerHeight, pixelY + playerHeight };

    for (int i = 0; i < 4; i++) {
        // Переводим пиксельные координаты в индексы нашей матрицы (grid)
        int cellX = static_cast<int>(cornersX[i] / tileSize);
        int cellY = static_cast<int>(cornersY[i] / tileSize);

        // Защита от выхода за пределы карты
        if (cellY < 0 || cellY >= grid.size() || cellX < 0 || cellX >= grid[cellY].size()) {
            return true; // Считаем края экрана стеной
        }

        // Если хоть один угол попал в стену '#' — возвращаем true
        if (grid[cellY][cellX] == '#') {
            return true;
        }
    }
    return false; // Путь свободен!
}

// === ШАГ 2.3: ИСЧЕЗНОВЕНИЕ МОНЕТОК ===
void Map::checkCoinCollision(float playerX, float playerY, float playerWidth, float playerHeight, int& score) {
    if (grid.empty()) return;

    // Находим центр игрока
    float centerX = playerX + playerWidth / 2.f;
    float centerY = playerY + playerHeight / 2.f;

    // Переводим в координаты сетки
    int cellX = static_cast<int>(centerX / tileSize);
    int cellY = static_cast<int>(centerY / tileSize);

    // Проверяем границы
    if (cellY >= 0 && cellY < grid.size() || cellX >= 0 || cellX < grid[cellY].size()) {
        // Если в этой клетке лежит монетка '.'
        if (grid[cellY][cellX] == '.') {
            grid[cellY][cellX] = ' '; // Меняем её на пустоту! Она исчезает
            score += 10;              // Добавляем очки (связка с feature/timer)
        }
    }
}

// === ОТРИСОВКА КАРТЫ ===
void Map::draw(sf::RenderWindow& window) {
    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            
            // Если стена — рисуем квадрат в нужной пиксельной координате
            if (grid[y][x] == '#') {
                wallShape.setPosition(x * tileSize, y * tileSize);
                window.draw(wallShape);
            }
            // Если монетка — рисуем кружок
            else if (grid[y][x] == '.') {
                coinShape.setPosition(x * tileSize, y * tileSize);
                window.draw(coinShape);
            }
        }
    }
}