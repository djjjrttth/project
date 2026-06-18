#include "Level.h"
#include <iostream>
#include <fstream>
#include <cstdlib> //для rand()

using namespace std;

/*LevelManager() {} //пока пустой

    //Функция считывания лабиринта из файла
    bool loadLevelFromFile(const string& filename); //проверяет возсожность записать лабиринт из тхт в curLabyrinth

    //Функция сборки лабиринта из нескольких шаблонов
    void generateRandomLevel(const vector<string>& templateFiles);

    //Передача Лабиринта в виде матрицы символов Лене для Map
    vector<vector<char>> getCurrentLayout() const; */


//Функция считывания лабиринта из файла
bool LevelManager::loadLevelFromFile(const string& filename) {
    ifstream file(filename); //открываем файл для чтения

    if (!fele.is_open()) {
        cerr<< "Error: No templates"<< endl;
        return false;
    }

    curLabyrinth.clear(); //очищаем старую сетку

    string line;

    //Читаем построчно пока строчки не кончатся
    while (getline(file, line)) { //читаем пока читается
        vector<char> row;
        for (char ch : line){ //по строке от первого символа до последнего
            row.push_back(ch);
        }

        curLabyrinth.push_back(row); //добавляет готовую стройку в массив
    }

    file.close();
    return true;
}

//Функция сборки лабиринта из нескольких шаблонов
void LevelManager::generateRandomLevel(const vector<string>& templateFiles) {
    levelTemplates.clear();

    for (const string& filename : templateFiles) {
        ifstream file(filename);
        if (file.is_open()) {
            vector<vector<char>> tempLabyrinth;
            string line;
            while(getline(file, line)) {
                vector<char> row(line.begin(), line.end());
                tempLabyrinth.push_back(row);
            }
            levelTemplates.push_back(tempLabyrinth);
            file.close();
        }
    }

    // если ни один шаблон не загрузился, выходим, чтобы программа не упала
    if (levelTemplates.empty()) {
        std::cerr << "Error: No templates loaded for random generation" << std::endl;
        return;
    }
    // очищаем текущую карту, сейчас будем склеивать её заново
    curLabyrinth.clear();

    //Склейка трёх случайных кусочков по вертикали
    for (int i = 0; i < 3; ++i) {
        // выбираем случайный индекс шаблона из улья
        int randomIndex = rand() % levelTemplates.size();
        const auto& chosenTemplate = levelTemplates[randomIndex];

        // Копируем строчки из выбранного шаблона в наш итоговый currentLayout
        for (const auto& row : chosenTemplate) {
            currentLayout.push_back(row);
        }
    }
}

// 3. Функция-поставщик: просто отдает наружу то, что лежит в currentLayout
std::vector<std::vector<char>> LevelManager::getCurrentLayout() const {
    return currentLayout;
}
