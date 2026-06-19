#pragma once

#include "Config.h"
#include "GameTimer.h"
#include "LevelManager.h"
#include "Map.h"
#include "Player.h"
#include "UI.h"

#include <SFML/Graphics.hpp>
#include <string>

class Game {
private:
    sf::RenderWindow window;
    sf::Clock frameClock;

    GameState state = GameState::Menu;
    GameMode mode = GameMode::Training;

    LevelManager levelManager;
    Map map;
    Player player;
    GameTimer timer;
    UI ui;

    int currentLevelIndex = 0;
    int visibleLevelNumber = 1;
    int score = 0;
    int bestEndlessScore = 0;
    int endlessSeed = 1;
    bool finalVictory = false;
    float transitionElapsed = 0.f;
    int menuSelection = 0;

public:
    Game();
    void run();

private:
    void processEvents();
    void handleKeyPress(sf::Keyboard::Key key);
    void update(float deltaSeconds);
    void render();

    void startTraining();
    void startEndless();
    void loadCurrentLevel(bool withTransition = true);
    void restartCurrentLevel();
    void goToNextLevelOrMenu();

    void applyCellEffect();
    void setGameOver();
    void setVictory();
    void confirmMenuSelection();
};
