#ifndef LEVEL_H  // 1. "Если НЕ объявлено имя LEVEL_H..."
#define LEVEL_H

#include <string>
#include <vector>
using namespace std;

class LevelManager {
private:

    vector<vector<char>> curLabyrinth; //двумерный массив с текущим лабиринтом

    vector<vector<vector<char>>> levelTemplates; //массив лабиринтов (для будующей склейки)

public:
    // Конструктор
    LevelManager() {} //пока пустой

    //Функция считывания лабиринта из файла
    bool loadLevelFromFile(const string& filename); //проверяет возсожность записать лабиринт из тхт в curLabyrinth

    //Функция сборки лабиринта из нескольких шаблонов
    void generateRandomLevel(const vector<string>& templateFiles);

    //Передача Лабиринта в виде матрицы символов Лене для Map
    vector<vector<char>> getCurrentLayout() const;
};