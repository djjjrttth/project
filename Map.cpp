#include "Map.h"

Map::Map() {}

void Map::loadFromLayout(const std::vector<std::vector<int>>& layout) {
    this->grid = layout;
    wallSprites.clear();
    coinSprites.clear(); 

    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == 1) { 
                sf::RectangleShape wall(sf::Vector2f(tileSize, tileSize));
                wall.setPosition(x * tileSize, y * tileSize);
                wall.setFillColor(sf::Color(0, 90, 255)); 
                wall.setOutlineThickness(-1.f); 
                wall.setOutlineColor(sf::Color(0, 45, 150));
                wallSprites.push_back(wall);
            } 
            else if (grid[y][x] == 0) { 
                sf::CircleShape coin(3.f); 
                coin.setFillColor(sf::Color(255, 215, 0)); 
                coin.setPosition(x * tileSize + 7.f, y * tileSize + 7.f);
                coinSprites.push_back(coin);
            }
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (const auto& wall : wallSprites) window.draw(wall);
    for (const auto& coin : coinSprites) window.draw(coin);
}

// ВОТ ЭТОТ МЕТОД ДОПИШИ В КОНЕЦ ФАЙЛА:
int Map::checkCoinCollision(float playerX, float playerY, float playerSize) {
    // Создаем невидимый прямоугольник вокруг игрока для проверки столкновений
    sf::FloatRect playerRect(playerX, playerY, playerSize, playerSize);
    int collectedCount = 0;

    // Идем по вектору монеток и удаляем те, которых коснулся игрок
    for (auto it = coinSprites.begin(); it != coinSprites.end(); ) {
        if (playerRect.intersects(it->getGlobalBounds())) {
            it = coinSprites.erase(it); // Монетка удаляется!
            collectedCount++;
        } else {
            ++it; // Иначе просто идем к следующей монетке
        }
    }
    return collectedCount; // Возвращаем количество съеденных за этот кадр монеток
}