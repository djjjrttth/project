#pragma once

#include <SFML/System/Vector2.hpp>

enum class Direction {
    None,
    Up,
    Down,
    Left,
    Right
};

inline sf::Vector2i directionToVector(Direction direction) {
    switch (direction) {
        case Direction::Up: return {0, -1};
        case Direction::Down: return {0, 1};
        case Direction::Left: return {-1, 0};
        case Direction::Right: return {1, 0};
        default: return {0, 0};
    }
}
