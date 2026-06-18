#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Map {
private:
    std::vector<std::vector<int>> grid;
    std::vector<sf::RectangleShape> wallSprites;
    std::vector<sf::CircleShape> coinSprites;
    float tileSize = 20.f; 

public:
    Map();
    void loadFromLayout(const std::vector<std::vector<int>>& layout);
    void draw(sf::RenderWindow& window);
  
    // ВОТ ЭТО ДОБАВИЛИ: метод проверяет, наступил ли игрок на монетку
    int checkCoinCollision(float playerX, float playerY, float playerSize);
};       