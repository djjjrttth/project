#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

template <int WIDTH, int HEIGHT>
class LevelManager {
private:
    // хранение координат стены/ячейки в списке границ
    struct Cell {
        int x, y;
    };

public:
    LevelManager() {
        srand(time(nullptr));
    }

    // главная функция генерации лабиринта по алгоритму Прима
    void generatePrimMaze(vector<vector<int>>& maze) {
        // 1.изначально всё заполнено стенами (1)
        maze.assign(HEIGHT, vector<int>(WIDTH, 1));

        // список «граничных» стен, границ
        vector<Cell> walls;

        // 2. выбор случайной стартовой точки внутри лабиринта 
        int startX = 1 + (rand() % ((WIDTH - 2) / 2)) * 2;
        int startY = 1 + (rand() % ((HEIGHT - 2) / 2)) * 2;
        
        maze[startY][startX] = 0; // делаем стартовую ячейку проходом

        //добавляем соседей стартовой точки в список стен
        addWalls(startX, startY, maze, walls);

        // направления для проверки соседей (на расстоянии 2 ячеек)
        int dx[] = { 0, 0, 2, -2 };
        int dy[] = { 2, -2, 0, 0 };

        // 3. основной цикл алгоритма
        while (!walls.empty()) {
            // выбор случайной стены из списка
            int randIdx = rand() % walls.size();
            Cell currentWall = walls[randIdx];

            // проверяем, сколько вокруг этой стены УЖЕ посещенных ячеек
            vector<Cell> visitedNeighbors;

            for (int i = 0; i < 4; ++i) {
                int nx = currentWall.x + dx[i];
                int ny = currentWall.y + dy[i];

                if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1) {
                    if (maze[ny][nx] == 0) {
                        visitedNeighbors.push_back({nx, ny});
                    }
                }
            }

            // если нашли ровно одного посещенного соседа, значит, эту стену можно разрушить
            if (visitedNeighbors.size() == 1) {
                Cell target = visitedNeighbors[0];
                
                // делаем проходом саму стену
                maze[currentWall.y][currentWall.x] = 0;
                
                // пробиваем путь между ними (среднюю ячейку)
                int midX = currentWall.x + (target.x - currentWall.x) / 2;
                int midY = currentWall.y + (target.y - currentWall.y) / 2;
                maze[midY][midX] = 0;

                // добавляем новые стены в список от текущей ячейки
                addWalls(currentWall.x, currentWall.y, maze, walls);
            }

            // удаляем обработанную стену из списка
            walls.erase(walls.begin() + randIdx);
        }

        // гарантируем, что по краям лабиринта всегда будут железные стены (границы карты)
        for (int x = 0; x < WIDTH; ++x) {
            maze[0][x] = 1;
            maze[HEIGHT - 1][x] = 1;
        }
        for (int y = 0; y < HEIGHT; ++y) {
            maze[y][0] = 1;
            maze[y][WIDTH - 1] = 1;
        }
    }

private:
    // вспомогательная функция для добавления соседних стен в список
    void addWalls(int x, int y, const vector<vector<int>>& maze, vector<Cell>& walls) {
        int dx[] = { 0, 0, 2, -2 };
        int dy[] = { 2, -2, 0, 0 };

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // проверяем, что не выходим за границы и там еще стена
            if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1) {
                if (maze[ny][nx] == 1) {
                    walls.push_back({nx, ny});
                }
            }
        }
    }
};

#endif