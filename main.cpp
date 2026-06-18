#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream> // Добавили для вывода счета в консоль
#include "Map.h"
#include "Level.h"
#include "Player.h" 

int main() {
    srand(static_cast<unsigned int>(time(0)));

    LevelManager<int> levelManager;
    levelManager.generateDynamicLevel(25, 19);

    Map gameMap;
    gameMap.loadFromLayout(levelManager.getCurrentLayout());

    Player player(20.f, 20.f, 14.f, 2.f); 

    int score = 0; // Переменная для подсчета собранных монеток!

    sf::RenderWindow window(sf::VideoMode(800, 600), "Tomb of the Mask - Collaboration Edition");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                levelManager.generateDynamicLevel(25, 19);
                gameMap.loadFromLayout(levelManager.getCurrentLayout());
                player = Player(20.f, 20.f, 14.f, 2.f); 
                score = 0; // Сбрасываем счет при генерации нового уровня
                std::cout << "New game started! Score reset.\n";
            }
        }

        // Движение игрока
        player.move(600, 800, levelManager.getCurrentLayout());

        // ВОТ ЭТО ДОБАВИЛИ: Каждый кадр проверяем, собрал ли игрок монетки
        // Передаем координаты из геттеров игрока и его размер (14.f)
        int collected = gameMap.checkCoinCollision(player.get_position_x(), player.get_position_y(), 14.f);
        if (collected > 0) {
            score += collected;
            std::cout << "Coins collected: " << score << "\n"; // Пишем счет в консоль разработчика
        }

        // Отрисовка
        window.clear(sf::Color(10, 10, 20));
        
        gameMap.draw(window);  
        player.draw(window);   
        
        window.display();
    }

    return 0;
}