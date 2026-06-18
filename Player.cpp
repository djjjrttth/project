#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


const int CELL_SIZE = 40;

Player::Player(float x, float y, float size, float speed, std::string filename)
    : speed(speed), filename(filename)
{   
    texture.loadFromFile(filename);
    sprite.setTexture(texture);
    
    position = sf::Vector2f(x, y);
    this->size = sf::Vector2f(size, size);

    sf::Vector2u texSize = texture.getSize();

    float targetWidth = 40;
    float targetHeight = 40;

    sprite.setScale(targetWidth / texSize.x, targetHeight / texSize.y);

    sprite.setPosition(position);
}



void Player::move(int const WINDOW_HEIGHT, int const WINDOW_WIDTH, const std::vector<std::vector<int>>& maze)
{   

    std::cout << sprite.getPosition().x << " " << sprite.getPosition().y << std::endl;
    int dx = 0, dy = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
        dy -= speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
        dy += speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
        dx -= speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
        dx += speed;


    int nextX = position.x + dx;

    int left   = nextX / CELL_SIZE;
    int right  = (nextX + size.x - 1) / CELL_SIZE;
    int top    = position.y / CELL_SIZE;
    int bottom = (position.y + size.y - 1) / CELL_SIZE;

    if (nextX >= 0 &&
        nextX + size.x <= WINDOW_WIDTH &&
        maze[top][left] == 0 &&
        maze[top][right] == 0 &&
        maze[bottom][left] == 0 &&
        maze[bottom][right] == 0){
        position.x = nextX;
    }


    int nextY = position.y + dy;

    left   = position.x / CELL_SIZE;
    right  = (position.x + size.x - 1) / CELL_SIZE;
    top    = nextY / CELL_SIZE;
    bottom = (nextY + size.y - 1) / CELL_SIZE;

    if (nextY >= 0 &&
        nextY + size.y <= WINDOW_HEIGHT &&
        maze[top][left] == 0 &&
        maze[top][right] == 0 &&
        maze[bottom][left] == 0 &&
        maze[bottom][right] == 0){
        position.y = nextY;
    }

    sprite.setPosition(position);
}

void Player::draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

int Player::get_position_x() const{
    return position.x;
}

int Player::get_position_y() const{
    return position.y;
}

void Player::set_position(int new_x, int new_y) {
    position.x = new_x;
    position.y = new_y;
    sprite.setPosition(position);
}