#pragma once

#include "Config.h"
#include "Direction.h"
#include "Map.h"

#include <SFML/Graphics.hpp>
#include <string>

class Player {
private:
    sf::Vector2i currentCell{1, 1};
    sf::Vector2f pixelPosition{0.f, 0.f};

    sf::Texture texture;
    sf::Sprite sprite;
    sf::RectangleShape fallbackShape;
    bool textureLoaded = false;

    bool canMove(Direction direction, const Map& map) const;
    void updateSpritePosition();

public:
    Player() = default;

    void loadTexture(const std::string& filename);
    void reset(sf::Vector2i startCell, const Map& map);
    bool move(Direction direction, const Map& map);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2i getCell() const;
};
