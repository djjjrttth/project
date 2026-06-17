#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Player::Player(float x, float y, float size, float speed): speed(speed) {
    position = sf::Vector2f(x, y);
    shape.setSize(sf::Vector2f(size, size));
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(size / 2.f, size / 2.f);
    shape.setPosition(position);
}

void Player::move(){
    std::cout << position.x << " " << position.y << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        if (position.y - 25 > 0)
            position.y -= speed;
        //shape.move(0.f, -speed);
        //std::cout << "W pressed!" << " " << speed << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        if (position.y + 25 < 600)
            position.y += speed;
        //std::cout << "S pressed!" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        if (position.x - 25 > 0)
            position.x -= speed;
        //std::cout << "A pressed!" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
       if (position.x + 25 < 800)
            position.x += speed;
        //std::cout << "D pressed!" << std::endl;
    }

    shape.setPosition(position);

}

void Player::draw(sf::RenderWindow& window) {
        window.draw(shape);
    }