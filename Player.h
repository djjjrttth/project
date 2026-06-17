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
        sf::Vector2f get_position() const;
        void set_position(int new_x, int new_y);
        void move(float delta_time);
        void draw(sf::RenderWindow& window);
};

#endif //PLAYER_H