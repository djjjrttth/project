#include "Map.h"
#include "Config.h"

#include <algorithm>
#include <iostream>

void Map::loadTextures(const std::string& wallTexturePath, const std::string& coinTexturePath) {
    wallTextureLoaded = wallTexture.loadFromFile(wallTexturePath);
    if (!wallTextureLoaded) {
        std::cerr << "Wall texture was not loaded: " << wallTexturePath << ". Fallback wall color will be used.\n";
    } else {
        wallTexture.setRepeated(false);
        wallTexture.setSmooth(false);
    }

    coinTextureLoaded = coinTexture.loadFromFile(coinTexturePath);
    if (!coinTextureLoaded) {
        std::cerr << "Coin texture was not loaded: " << coinTexturePath << ". Fallback coin shape will be used.\n";
    } else {
        coinTexture.setSmooth(true);
    }
}

bool Map::loadFromLayout(const std::vector<std::string>& layout) {
    if (layout.empty()) {
        return false;
    }

    const std::size_t width = layout.front().size();
    if (width == 0) {
        return false;
    }

    for (const std::string& row : layout) {
        if (row.size() != width) {
            return false;
        }
    }

    grid = layout;
    cellSize = Config::CellSize;
    startCell = {1, 1};
    exitCell = {1, 1};

    for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
        for (int x = 0; x < static_cast<int>(grid[y].size()); ++x) {
            if (grid[y][x] == 'P') {
                startCell = {x, y};
                grid[y][x] = ' ';
            }
            if (grid[y][x] == 'E') {
                exitCell = {x, y};
            }
        }
    }

    return true;
}

void Map::draw(sf::RenderWindow& window) const {
    for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
        for (int x = 0; x < static_cast<int>(grid[y].size()); ++x) {
            drawFloor(window, x, y);

            switch (grid[y][x]) {
                case '#': drawWall(window, x, y); break;
                case '.': drawCoin(window, x, y); break;
                case 'B': drawBonus(window, x, y); break;
                case 'S': drawSpike(window, x, y); break;
                case 'E': drawExit(window, x, y); break;
                default: break;
            }
        }
    }
}

bool Map::isInside(sf::Vector2i cell) const {
    return cell.y >= 0 && cell.y < static_cast<int>(grid.size()) &&
           cell.x >= 0 && !grid.empty() && cell.x < static_cast<int>(grid[cell.y].size());
}

bool Map::isWall(sf::Vector2i cell) const {
    if (!isInside(cell)) {
        return true;
    }
    return grid[cell.y][cell.x] == '#';
}

bool Map::isWalkable(sf::Vector2i cell) const {
    return isInside(cell) && !isWall(cell);
}

bool Map::isExit(sf::Vector2i cell) const {
    return isInside(cell) && grid[cell.y][cell.x] == 'E';
}

CollectResult Map::collectAt(sf::Vector2i cell) {
    CollectResult result;
    if (!isInside(cell)) {
        return result;
    }

    char& tile = grid[cell.y][cell.x];
    if (tile == '.') {
        result.points += Config::CoinPoints;
        tile = ' ';
    } else if (tile == 'B') {
        result.points += Config::BonusPoints;
        result.bonusSeconds += Config::TimeBonusSeconds;
        tile = ' ';
    } else if (tile == 'S') {
        result.hitSpike = true;
    }

    return result;
}

sf::Vector2i Map::getStartCell() const {
    return startCell;
}

sf::Vector2i Map::getExitCell() const {
    return exitCell;
}

int Map::getCellSize() const {
    return cellSize;
}

sf::Vector2f Map::cellToPixel(sf::Vector2i cell) const {
    return {
        static_cast<float>(cell.x * cellSize),
        static_cast<float>(cell.y * cellSize + Config::TopPanelHeight)
    };
}

const std::vector<std::string>& Map::getGrid() const {
    return grid;
}

void Map::drawFloor(sf::RenderWindow& window, int x, int y) const {
    sf::RectangleShape floor(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
    floor.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize + Config::TopPanelHeight));

    const bool even = ((x + y) % 2 == 0);
    floor.setFillColor(even ? sf::Color(124, 99, 26) : sf::Color(116, 92, 22));
    window.draw(floor);

    sf::RectangleShape shade(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
    shade.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize + Config::TopPanelHeight));
    shade.setFillColor(sf::Color(0, 0, 0, 18));
    shade.setOutlineThickness(-1.f);
    shade.setOutlineColor(sf::Color(150, 120, 36, 30));
    window.draw(shade);
}

void Map::drawWall(sf::RenderWindow& window, int x, int y) const {
    if (wallTextureLoaded) {
        sf::Sprite wall;
        wall.setTexture(wallTexture);
        const sf::Vector2u textureSize = wallTexture.getSize();
        if (textureSize.x > 0 && textureSize.y > 0) {
            wall.setScale(
                static_cast<float>(cellSize) / static_cast<float>(textureSize.x),
                static_cast<float>(cellSize) / static_cast<float>(textureSize.y)
            );
        }
        wall.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize + Config::TopPanelHeight));
        window.draw(wall);

        sf::RectangleShape shade(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
        shade.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize + Config::TopPanelHeight));
        shade.setFillColor(sf::Color(0, 0, 0, 35));
        shade.setOutlineThickness(-1.f);
        shade.setOutlineColor(sf::Color(94, 98, 54, 120));
        window.draw(shade);
        return;
    }

    sf::RectangleShape wall(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
    wall.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize + Config::TopPanelHeight));
    wall.setFillColor(sf::Color(156, 163, 96));
    wall.setOutlineThickness(-2.f);
    wall.setOutlineColor(sf::Color(94, 98, 54));
    window.draw(wall);
}

void Map::drawCoin(sf::RenderWindow& window, int x, int y) const {
    if (coinTextureLoaded) {
        sf::Sprite coin;
        coin.setTexture(coinTexture);
        const sf::Vector2u textureSize = coinTexture.getSize();
        const float targetSize = Config::CoinVisualSize;
        if (textureSize.x > 0 && textureSize.y > 0) {
            coin.setScale(targetSize / static_cast<float>(textureSize.x), targetSize / static_cast<float>(textureSize.y));
        }
        coin.setPosition(
            static_cast<float>(x * cellSize) + (static_cast<float>(cellSize) - targetSize) / 2.f,
            static_cast<float>(y * cellSize + Config::TopPanelHeight) + (static_cast<float>(cellSize) - targetSize) / 2.f
        );
        window.draw(coin);
        return;
    }

    sf::CircleShape coin(static_cast<float>(cellSize) * 0.28f);
    coin.setPosition(
        static_cast<float>(x * cellSize) + cellSize * 0.22f,
        static_cast<float>(y * cellSize + Config::TopPanelHeight) + cellSize * 0.22f
    );
    coin.setFillColor(sf::Color(255, 215, 70));
    coin.setOutlineThickness(1.5f);
    coin.setOutlineColor(sf::Color(255, 245, 170));
    window.draw(coin);
}

void Map::drawBonus(sf::RenderWindow& window, int x, int y) const {
    sf::CircleShape bonus(static_cast<float>(cellSize) * 0.27f, 6);
    bonus.setPosition(
        static_cast<float>(x * cellSize) + cellSize * 0.23f,
        static_cast<float>(y * cellSize + Config::TopPanelHeight) + cellSize * 0.23f
    );
    bonus.setFillColor(sf::Color(60, 225, 255));
    bonus.setOutlineThickness(2.f);
    bonus.setOutlineColor(sf::Color::White);
    window.draw(bonus);
}

void Map::drawSpike(sf::RenderWindow& window, int x, int y) const {
    sf::ConvexShape spike;
    spike.setPointCount(3);
    spike.setPoint(0, sf::Vector2f(static_cast<float>(x * cellSize + cellSize / 2), static_cast<float>(y * cellSize + Config::TopPanelHeight + 4)));
    spike.setPoint(1, sf::Vector2f(static_cast<float>(x * cellSize + 4), static_cast<float>(y * cellSize + Config::TopPanelHeight + cellSize - 4)));
    spike.setPoint(2, sf::Vector2f(static_cast<float>(x * cellSize + cellSize - 4), static_cast<float>(y * cellSize + Config::TopPanelHeight + cellSize - 4)));
    spike.setFillColor(sf::Color(230, 50, 65));
    spike.setOutlineThickness(1.f);
    spike.setOutlineColor(sf::Color(255, 180, 180));
    window.draw(spike);
}

void Map::drawExit(sf::RenderWindow& window, int x, int y) const {
    sf::RectangleShape outer(sf::Vector2f(static_cast<float>(cellSize - 4), static_cast<float>(cellSize - 4)));
    outer.setPosition(static_cast<float>(x * cellSize + 2), static_cast<float>(y * cellSize + Config::TopPanelHeight + 2));
    outer.setFillColor(sf::Color(72, 55, 15));
    outer.setOutlineThickness(2.f);
    outer.setOutlineColor(sf::Color(255, 220, 95));
    window.draw(outer);

    sf::RectangleShape inner(sf::Vector2f(static_cast<float>(cellSize - 14), static_cast<float>(cellSize - 10)));
    inner.setPosition(static_cast<float>(x * cellSize + 7), static_cast<float>(y * cellSize + Config::TopPanelHeight + 5));
    inner.setFillColor(sf::Color(28, 20, 6));
    window.draw(inner);

    sf::CircleShape handle(2.7f);
    handle.setFillColor(sf::Color(235, 210, 120));
    handle.setPosition(static_cast<float>(x * cellSize + cellSize - 12), static_cast<float>(y * cellSize + Config::TopPanelHeight + cellSize / 2));
    window.draw(handle);
}
