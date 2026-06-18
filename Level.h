#ifndef LEVEL_H  // 1. "Если НЕ объявлено имя LEVEL_H..."
#define LEVEL_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <random>
#include <stack>
#include <algorithm>
#include <type_traits>
using namespace std;

template <typename T>
class LevelManager {
private:
    vector<vector<T>> currentLayout;
    vector<vector<vector<T>>> levelTemplates;

public:
    //МЕТОДЫ ДЛЯ РЕЖИМА 1 (ОБУЧЕНИЕ)
    // Они независимы, лежат здесь и готовы к использованию в любой момент

    bool loadLevelFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }

        currentLayout.clear();
        string line;
        while (getline(file, line)) {
            vector<T> row;
            for (char ch : line) {
                if constexpr (is_same_v<T, int>) {
                    row.push_back(ch == '#' ? 1 : 0);
                } else {
                    row.push_back(ch);
                }
            }
            currentLayout.push_back(row);
        }
        file.close();
        return true;
    }

    void generateRandomLevel(const vector<string>& templateFiles) {
        levelTemplates.clear();
        for (const string& filename : templateFiles) {
            ifstream file(filename);
            if (file.is_open()) {
                vector<vector<T>> tempLayout;
                string line;
                while (getline(file, line)) {
                    vector<T> row;
                    for (char ch : line) {
                        if constexpr (is_same_v<T, int>) {
                            row.push_back(ch == '#' ? 1 : 0);
                        } else {
                            row.push_back(ch);
                        }
                    }
                    tempLayout.push_back(row);
                }
                levelTemplates.push_back(tempLayout);
                file.close();
            }
        }

        if (levelTemplates.empty()) {
            cerr << "Error: No templates loaded for random generation!" << endl;
            return;
        }

        currentLayout.clear();
        for (int i = 0; i < 3; ++i) {
            int randomIndex = rand() % levelTemplates.size();
            for (const auto& row : levelTemplates[randomIndex]) {
                currentLayout.push_back(row);
            }
        }
    }


    // МЕТОД ДЛЯ РЕЖИМА 2 (БЕСКОНЕЧНЫЙ ЛАБИРИНТ - АЛГОРИТМ КАТИ)

    void generateDynamicLevel(int width, int height) {
        // Внутренний алгоритм считает всё в int (Катина логика)
        vector<vector<int>> intGrid(height, vector<int>(width, 1));

        int startX = 1;
        int startY = 1;
        intGrid[startY][startX] = 0;

        stack<std::pair<int, int>> stack;
        stack.push({startX, startY});

        random_device rd;
        mt19937 g(rd());
        vector<pair<int, int>> directions = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

        while (!stack.empty()) {
            auto [x, y] = stack.top();
            shuffle(directions.begin(), directions.end(), g);

            bool found = false;
            for (auto [dx, dy] : directions) {
                int nx = x + dx;
                int ny = y + dy;

                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && intGrid[ny][nx] == 1) {
                    intGrid[y + dy / 2][x + dx / 2] = 0;
                    intGrid[ny][nx] = 0;
                    stack.push({nx, ny});
                    found = true;
                    break;
                }
            }
            if (!found) {
                stack.pop();
            }
        }

        // Вход и выход
        intGrid[1][0] = 0;
        intGrid[height - 2][width - 1] = 0;

        // Конвертируем в итоговый формат currentLayout в зависимости от типа T
        currentLayout.clear();
        for (int y = 0; y < height; ++y) {
            vector<T> row;
            for (int x = 0; x < width; ++x) {
                if constexpr (is_same_v<T, int>) {
                    row.push_back(intGrid[y][x]); // Если нужен int (1 или 0)
                } else {
                    row.push_back(intGrid[y][x] == 1 ? '#' : ' '); // Если нужен char ('#' или ' ')
                }
            }
            currentLayout.push_back(row);
        }
    }

    // Универсальный геттер для получения сетки уровня
    vector<vector<T>> getCurrentLayout() const {
        return currentLayout;
    }
};

#endif