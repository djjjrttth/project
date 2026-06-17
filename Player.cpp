#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Player::Player(float x, float y, float size, float speed){
    position = sf::Vector2f(x, y);
    shape.setSize(sf::Vector2f(size, size));
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(size / 2.f, size / 2.f);
    shape.setPosition(position);
}

void Player::move(float deltaTime){
    
    

        // Обработка нажатий клавиш
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
        shape.move(0.f, -5.f);
        //std::cout << "W pressed!" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
        shape.move(0.f, 5.f);
        //std::cout << "S pressed!" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
        shape.move(-5.f, 0.f);
        //std::cout << "A pressed!" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
        shape.move(5.f, 0.f);
        //std::cout << "D pressed!" << std::endl;
    }

        // Применяем движение с учетом deltaTime и скорости
        
}

void Player::draw(sf::RenderWindow& window) {
        window.draw(shape);
    }