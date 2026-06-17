#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player{
    private:
        int direction;
        float speed;
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Sprite sprite;
        sf::RectangleShape shape;
        

    public:
        Player(float x, float y, float size, float speed);
        float get_position_x() const;
        float get_position_y() const;
        void set_position(int new_x, int new_y);
        void move();
        void draw(sf::RenderWindow& window);
};

#endif //PLAYER_H