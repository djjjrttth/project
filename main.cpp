#include <SFML/Graphics.hpp>
#include "Player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Project");
    window.setFramerateLimit(60);

    Player player(400.f, 300.f, 50.f, 5);
    sf::Clock clock;

    while (window.isOpen()) {
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.move();

        window.clear(sf::Color::Black);
        player.draw(window);
        window.display();
    }

    return 0;
}