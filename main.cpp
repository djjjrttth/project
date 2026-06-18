#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream> // Добавили для вывода счета в консоль
#include "Map.h"
#include "Level.h"
#include "Player.h" 
#include "Settings.h"

#include <vector>
#include <iostream>
#include <random>
#include <stack>
#include <sstream>   


// Настройки игры



// ===== ДЛЯ ТАЙМЕРА =====
const float LEVEL_TIME_LIMIT = 30.f;   // секунд на уровень
int level = 1;
int score = 0;
float timeLeft = LEVEL_TIME_LIMIT;
bool victory = false;
bool gameOver = false;
sf::Clock levelClock;


void generateMaze(std::vector<std::vector<int>>& grid);

// ===== ФУНКЦИЯ СБРОСА УРОВНЯ =====
void resetLevel(Player& player, LevelManager<int>& levelManager, Map& gameMap) {

    levelManager.generateDynamicLevel(GRID_HEIGHT, GRID_WIDTH);
    gameMap.loadFromLayout(levelManager.getCurrentLayout());
    // generateMaze(maze);
    // walls.clear();
    // sf::Color wallColor(50, 50, 50);
    // for (int y = 0; y < GRID_HEIGHT; ++y) {
    //     for (int x = 0; x < GRID_WIDTH; ++x) {
    //         if (maze[y][x] == 1) {
    //             sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    //             wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
    //             wall.setFillColor(wallColor);
    //             walls.push_back(wall);
    //         }
    //     }
    // }
    player.set_position(START_X, START_Y);
    levelClock.restart();
    timeLeft = LEVEL_TIME_LIMIT;
    victory = false;
    gameOver = false;
    score = 0;
}
// ===== 


//Функция для генерации лабиринта алгоритмом DFS (Recursive Backtracker)
// void generateMaze(std::vector<std::vector<int>>& grid) {
//     // 1. Заполняем всё стенами
//     for (int y = 0; y < GRID_HEIGHT; ++y) {
//         for (int x = 0; x < GRID_WIDTH; ++x) {
//             grid[y][x] = 1; 
//         }
//     }

//     // 2. Начинаем "прорубать" проходы с клетки (1, 1)
//     int startX = 1;
//     int startY = 1;
//     grid[startY][startX] = 0;

//     std::stack<std::pair<int, int>> stack;
//     stack.push({startX, startY});

//     // Для генерации случайных чисел
//     std::random_device rd;
//     std::mt19937 g(rd());

//     // Направления: вверх, вниз, влево, вправо (шаг 2, чтобы перепрыгнуть стену)
//     std::vector<std::pair<int, int>> directions = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

//     while (!stack.empty()) {
//         auto [x, y] = stack.top();
        
//         // Перемешиваем направления для случайности
//         std::shuffle(directions.begin(), directions.end(), g);

//         bool found = false;
//         for (auto [dx, dy] : directions) {
//             int nx = x + dx;
//             int ny = y + dy;

//             // Проверяем, не вышли ли мы за границы и не является ли там уже проходом
//             if (nx > 0 && nx < GRID_WIDTH - 1 && ny > 0 && ny < GRID_HEIGHT - 1 && grid[ny][nx] == 1) {
//                 // Прорубаем стену между текущей и следующей клеткой
//                 grid[y + dy / 2][x + dx / 2] = 0;
//                 // Делаем следующую клетку проходом
//                 grid[ny][nx] = 0;
                
//                 stack.push({nx, ny});
//                 found = true;
//                 break; // Идем дальше из новой клетки
//             }
//         }

//         if (!found) {
//             stack.pop(); // Возвращаемся назад, если тупик
//         }
//     }
    
//     // Делаем вход и выход (опционально)
//     grid[1][0] = 0; // Вход слева
//     grid[GRID_HEIGHT - 2][GRID_WIDTH - 1] = 0; // Выход справа
// }


int main() {
    srand(static_cast<unsigned int>(time(0)));

    LevelManager<int> levelManager;
    levelManager.generateDynamicLevel(GRID_HEIGHT, GRID_WIDTH);

    Map gameMap;
    gameMap.loadFromLayout(levelManager.getCurrentLayout());

    //int score = 0; // Переменная для подсчета собранных монеток!

    //sf::RenderWindow window(sf::VideoMode(800, 600), "Tomb of the Mask - Collaboration Edition");
    

    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Project");
    window.setFramerateLimit(60);
    
    // ===== ЗАГРУЗКА ШРИФТА И ТЕКСТОВ =====
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Warning: arial.ttf not found! UI text will not be displayed.\n";
    }
    sf::Text timeText, scoreText, levelText, messageText;
    timeText.setFont(font);      timeText.setCharacterSize(24); timeText.setFillColor(sf::Color::White); timeText.setPosition(10, 10);
    scoreText.setFont(font);     scoreText.setCharacterSize(24); scoreText.setFillColor(sf::Color::Yellow); scoreText.setPosition(10, 40);
    levelText.setFont(font);     levelText.setCharacterSize(24); levelText.setFillColor(sf::Color::Cyan); levelText.setPosition(10, 70);
    messageText.setFont(font);   messageText.setCharacterSize(48); messageText.setFillColor(sf::Color::Red); messageText.setPosition(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 50);
    // ===== 
    

    //std::vector<std::vector<int>> maze(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 1));
    //generateMaze(maze);

    //std::vector<sf::RectangleShape> walls;
    //sf::Color wallColor(50, 50, 50); // Темно-серый цвет для стен
    //sf::Color bgColor(240, 240, 240); // Светлый фон

    //for (int y = 0; y < GRID_HEIGHT; ++y) {
    //    for (int x = 0; x < GRID_WIDTH; ++x) {
    //        std::cout << maze[y][x];
    //    }
    //    std::cout << std::endl;
    //}

    //for (int y = 0; y < GRID_HEIGHT; ++y) {
    //    for (int x = 0; x < GRID_WIDTH; ++x) {
    //        if (maze[y][x] == 1) {
    //            sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    //            wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
    //            wall.setFillColor(wallColor);
    //            walls.push_back(wall);
    //        }
    //     }
    //}

    Player player(START_X, START_Y, PLAYER_SIZE, PLAYER_SPEED, FILENAME_PLAYER);
    //resetLevel(player, maze, walls);
    
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            // ===== обработка клавиш для победы/поражения
            if (victory && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                level++;
                //score += 10;
                resetLevel(player, levelManager, gameMap);
            }
            if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                resetLevel(player, levelManager, gameMap);
            }
            // ===== 

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                levelManager.generateDynamicLevel(GRID_HEIGHT, GRID_WIDTH);
                gameMap.loadFromLayout(levelManager.getCurrentLayout());
                //player = Player(20.f, 20.f, 14.f, 2.f); 
                score = 0; // Сбрасываем счет при генерации нового уровня
                std::cout << "New game started! Score reset.\n";
            }
        }

        // Движение игрока
        player.move(WINDOW_HEIGHT, WINDOW_WIDTH, levelManager.getCurrentLayout());

        // ВОТ ЭТО ДОБАВИЛИ: Каждый кадр проверяем, собрал ли игрок монетки
        // Передаем координаты из геттеров игрока и его размер (14.f)
        int collected = gameMap.checkCoinCollision(player.get_position_x(), player.get_position_y(), PLAYER_SIZE);
        if (collected > 0) {
            score += collected;
            std::cout << "Coins collected: " << score << "\n"; // Пишем счет в консоль разработчика
        }

        // Отрисовка
        window.clear(sf::Color(10, 10, 20));
        
        gameMap.draw(window);  
        player.draw(window);   
        
        //generateMaze(maze);
        //walls.clear();
        // for (int y = 0; y < GRID_HEIGHT; ++y) {
        //     for (int x = 0; x < GRID_WIDTH; ++x) {
        //         if (maze[y][x] == 1) {
        //             sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        //             wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
        //             wall.setFillColor(wallColor);
        //             walls.push_back(wall);
        //         }
        //     }
        // }
            
        

        // ===== ЛОГИКА ТАЙМЕРА =====
        if (!victory && !gameOver) {
            float elapsed = levelClock.getElapsedTime().asSeconds();
            timeLeft = LEVEL_TIME_LIMIT - elapsed;
            if (timeLeft < 0) timeLeft = 0;

            if (timeLeft <= 0) {
                gameOver = true;
                messageText.setString("GAME OVER\nPress R to retry");
                messageText.setFillColor(sf::Color::Red);
            }

            // Проверка победы — когда игрок на выходе (координаты (GRID_WIDTH-1, GRID_HEIGHT-2))
            int playerCellX = player.get_position_x() / CELL_SIZE;
            int playerCellY = player.get_position_y() / CELL_SIZE;
            if (playerCellX == GRID_WIDTH-1 && playerCellY == GRID_HEIGHT-2) {
                victory = true;
                messageText.setString("VICTORY!\nPress Enter for next level");
                messageText.setFillColor(sf::Color::Green);
            }

            // Обновление UI
            std::stringstream ss;
            ss << "Time: " << (int)timeLeft;
            timeText.setString(ss.str());
            scoreText.setString("Score: " + std::to_string(score));
            levelText.setString("Level: " + std::to_string(level));
        }
        // ===== 


        //player.move(WINDOW_HEIGHT, WINDOW_WIDTH, maze);

        //window.clear(sf::Color::Black);
        //player.draw(window);
        //for (const auto& wall : walls) {
        //    window.draw(wall);
        //}

        


        // ===== ОТРИСОВКА UI =====
        window.draw(timeText);
        window.draw(scoreText);
        window.draw(levelText);
        if (victory || gameOver) {
            window.draw(messageText);
        }
        // ===== 


        window.display();
    }

    return 0;
}