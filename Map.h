#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct CollectResult {
    int points = 0;
    float bonusSeconds = 0.f;
    bool hitSpike = false;
};

class Map {
private:
    std::vector<std::string> grid;
    sf::Vector2i startCell{1, 1};
    sf::Vector2i exitCell{1, 1};
    int cellSize = 32;

    sf::Texture wallTexture;
    sf::Texture coinTexture;
    bool wallTextureLoaded = false;
    bool coinTextureLoaded = false;

    void drawFloor(sf::RenderWindow& window, int x, int y) const;
    void drawWall(sf::RenderWindow& window, int x, int y) const;
    void drawCoin(sf::RenderWindow& window, int x, int y) const;
    void drawBonus(sf::RenderWindow& window, int x, int y) const;
    void drawSpike(sf::RenderWindow& window, int x, int y) const;
    void drawExit(sf::RenderWindow& window, int x, int y) const;

public:
    Map() = default;

    void loadTextures(const std::string& wallTexturePath, const std::string& coinTexturePath);
    bool loadFromLayout(const std::vector<std::string>& layout);
    void draw(sf::RenderWindow& window) const;

    bool isInside(sf::Vector2i cell) const;
    bool isWall(sf::Vector2i cell) const;
    bool isWalkable(sf::Vector2i cell) const;
    bool isExit(sf::Vector2i cell) const;

    CollectResult collectAt(sf::Vector2i cell);

    sf::Vector2i getStartCell() const;
    sf::Vector2i getExitCell() const;
    int getCellSize() const;
    sf::Vector2f cellToPixel(sf::Vector2i cell) const;
    const std::vector<std::string>& getGrid() const;
};
