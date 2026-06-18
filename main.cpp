#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream> // Добавили для вывода счета в консоль
#include "Map.h"
#include "Level.h"
#include "Player.h" 

#include <vector>
#include <iostream>
#include <random>
#include <stack>


// Настройки игры
const int GRID_WIDTH = 21;   
const int GRID_HEIGHT = 21;
const int CELL_SIZE = 40;
const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;

const int PLAYER_SIZE = 40;
const int PLAYER_SPEED = 10;


const float START_X = 0;
const float START_Y = CELL_SIZE;
const std::string FILENAME_PLAYER = "src/player.png"; 

//Функция для генерации лабиринта алгоритмом DFS (Recursive Backtracker)
void generateMaze(std::vector<std::vector<int>>& grid) {
    // 1. Заполняем всё стенами
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            grid[y][x] = 1; 
        }
    }

    // 2. Начинаем "прорубать" проходы с клетки (1, 1)
    int startX = 1;
    int startY = 1;
    grid[startY][startX] = 0;

    std::stack<std::pair<int, int>> stack;
    stack.push({startX, startY});

    // Для генерации случайных чисел
    std::random_device rd;
    std::mt19937 g(rd());

    // Направления: вверх, вниз, влево, вправо (шаг 2, чтобы перепрыгнуть стену)
    std::vector<std::pair<int, int>> directions = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

    while (!stack.empty()) {
        auto [x, y] = stack.top();
        
        // Перемешиваем направления для случайности
        std::shuffle(directions.begin(), directions.end(), g);

        bool found = false;
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            // Проверяем, не вышли ли мы за границы и не является ли там уже проходом
            if (nx > 0 && nx < GRID_WIDTH - 1 && ny > 0 && ny < GRID_HEIGHT - 1 && grid[ny][nx] == 1) {
                // Прорубаем стену между текущей и следующей клеткой
                grid[y + dy / 2][x + dx / 2] = 0;
                // Делаем следующую клетку проходом
                grid[ny][nx] = 0;
                
                stack.push({nx, ny});
                found = true;
                break; // Идем дальше из новой клетки
            }
        }

        if (!found) {
            stack.pop(); // Возвращаемся назад, если тупик
        }
    }
    
    // Делаем вход и выход (опционально)
    grid[1][0] = 0; // Вход слева
    grid[GRID_HEIGHT - 2][GRID_WIDTH - 1] = 0; // Выход справа
}


int main() {
    srand(static_cast<unsigned int>(time(0)));

    LevelManager<int> levelManager;
    levelManager.generateDynamicLevel(25, 19);

    Map gameMap;
    gameMap.loadFromLayout(levelManager.getCurrentLayout());

    Player player(20.f, 20.f, 14.f, 2.f); 

    int score = 0; // Переменная для подсчета собранных монеток!

    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Backrooms");
    window.setFramerateLimit(60);

    std::vector<std::vector<int>> maze(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 1));
    generateMaze(maze);

    std::vector<sf::RectangleShape> walls;
    sf::Color wallColor(50, 50, 50); // Темно-серый цвет для стен
    sf::Color bgColor(240, 240, 240); // Светлый фон

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (maze[y][x] == 1) {
                sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                wall.setFillColor(wallColor);
                walls.push_back(wall);
            }
        }
    }

    Player player(START_X, START_Y, PLAYER_SIZE, PLAYER_SPEED, FILENAME_PLAYER);
    
    sf::Clock clock;

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
        
                generateMaze(maze);
                walls.clear();
                for (int y = 0; y < GRID_HEIGHT; ++y) {
                    for (int x = 0; x < GRID_WIDTH; ++x) {
                        if (maze[y][x] == 1) {
                            sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                            wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                            wall.setFillColor(wallColor);
                            walls.push_back(wall);
                        }
                    }
                }
            }
        }


        player.move(WINDOW_HEIGHT, WINDOW_WIDTH, maze);

        window.clear(sf::Color::Black);
        player.draw(window);
        for (const auto& wall : walls) {
            window.draw(wall);
        }
        window.display();
    }

    return 0;
}