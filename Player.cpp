#include "Player.h"
#include "Config.h"

#include <iostream>

void Player::loadTexture(const std::string& filename) {
    textureLoaded = texture.loadFromFile(filename);
    if (!textureLoaded) {
        std::cerr << "Player texture was not loaded: " << filename << ". Fallback square will be used.\n";
    }

    if (textureLoaded) {
        sprite.setTexture(texture);
        sf::Vector2u textureSize = texture.getSize();
        if (textureSize.x > 0 && textureSize.y > 0) {
            const float targetSize = Config::PlayerVisualSize;
            sprite.setScale(targetSize / static_cast<float>(textureSize.x), targetSize / static_cast<float>(textureSize.y));
        }
    }

    fallbackShape.setSize(sf::Vector2f(Config::PlayerVisualSize, Config::PlayerVisualSize));
    fallbackShape.setFillColor(sf::Color(255, 120, 70));
    fallbackShape.setOutlineThickness(2.f);
    fallbackShape.setOutlineColor(sf::Color::White);
}

void Player::reset(sf::Vector2i startCell, const Map& map) {
    currentCell = startCell;
    pixelPosition = map.cellToPixel(startCell);
    updateSpritePosition();
}

bool Player::move(Direction direction, const Map& map) {
    if (!canMove(direction, map)) {
        return false;
    }

    const sf::Vector2i offset = directionToVector(direction);
    currentCell = {currentCell.x + offset.x, currentCell.y + offset.y};
    pixelPosition = map.cellToPixel(currentCell);
    updateSpritePosition();
    return true;
}

void Player::draw(sf::RenderWindow& window) const {
    if (textureLoaded) {
        window.draw(sprite);
    } else {
        window.draw(fallbackShape);
    }
}

sf::Vector2i Player::getCell() const {
    return currentCell;
}

bool Player::canMove(Direction direction, const Map& map) const {
    if (direction == Direction::None) {
        return false;
    }

    const sf::Vector2i offset = directionToVector(direction);
    const sf::Vector2i nextCell(currentCell.x + offset.x, currentCell.y + offset.y);
    return map.isWalkable(nextCell);
}

void Player::updateSpritePosition() {
    const float offset = (static_cast<float>(Config::CellSize) - Config::PlayerVisualSize) / 2.f;
    if (textureLoaded) {
        sprite.setPosition(pixelPosition.x + offset, pixelPosition.y + offset);
    }
    fallbackShape.setPosition(pixelPosition.x + offset, pixelPosition.y + offset);
}
