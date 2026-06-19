#include "Game.h"
#include "ResourcePath.h"

#include <SFML/Window/Event.hpp>
#include <iostream>

namespace {
    Direction keyToDirection(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::W || key == sf::Keyboard::Up) {
            return Direction::Up;
        }
        if (key == sf::Keyboard::S || key == sf::Keyboard::Down) {
            return Direction::Down;
        }
        if (key == sf::Keyboard::A || key == sf::Keyboard::Left) {
            return Direction::Left;
        }
        if (key == sf::Keyboard::D || key == sf::Keyboard::Right) {
            return Direction::Right;
        }
        return Direction::None;
    }
}

Game::Game()
    : window(sf::VideoMode(Config::WindowWidth, Config::WindowHeight), Config::WindowTitle),
      timer(Config::LevelTimeLimit) {
    window.setFramerateLimit(60);

    ui.loadDefaultFont();
    ui.loadMenuBackground(findExistingPath(Config::MenuBackgroundPath));
    player.loadTexture(findExistingPath(Config::PlayerTexturePath));
    map.loadTextures(findExistingPath(Config::WallTexturePath), findExistingPath(Config::CoinTexturePath));

    std::cout << "Tomb of the Mask - Student Edition started.\n";
    std::cout << "Controls: menu W/S or arrows + Enter, gameplay WASD/arrows, R restart, Esc menu.\n";
}

void Game::run() {
    while (window.isOpen()) {
        const float deltaSeconds = frameClock.restart().asSeconds();
        processEvents();
        update(deltaSeconds);
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape) {
        state = GameState::Menu;
        return;
    }

    if (state == GameState::Menu) {
        if (key == sf::Keyboard::W || key == sf::Keyboard::Up) {
            menuSelection = (menuSelection + 2) % 3;
        } else if (key == sf::Keyboard::S || key == sf::Keyboard::Down) {
            menuSelection = (menuSelection + 1) % 3;
        } else if (key == sf::Keyboard::Enter) {
            confirmMenuSelection();
        } else if (key == sf::Keyboard::Num1 || key == sf::Keyboard::Numpad1) {
            menuSelection = 0;
            confirmMenuSelection();
        } else if (key == sf::Keyboard::Num2 || key == sf::Keyboard::Numpad2) {
            menuSelection = 1;
            confirmMenuSelection();
        } else if (key == sf::Keyboard::Num3 || key == sf::Keyboard::Numpad3) {
            menuSelection = 2;
            confirmMenuSelection();
        }
        return;
    }

    if (state == GameState::Transition) {
        return;
    }

    if (state == GameState::Playing) {
        if (key == sf::Keyboard::R) {
            restartCurrentLevel();
            return;
        }

        const Direction direction = keyToDirection(key);
        if (direction != Direction::None) {
            if (player.move(direction, map)) {
                applyCellEffect();
                if (map.isExit(player.getCell())) {
                    setVictory();
                }
            }
        }
        return;
    }

    if (state == GameState::GameOver) {
        if (key == sf::Keyboard::R) {
            restartCurrentLevel();
        }
        return;
    }

    if (state == GameState::Victory) {
        if (key == sf::Keyboard::Enter) {
            goToNextLevelOrMenu();
        }
    }
}

void Game::update(float deltaSeconds) {
    if (state == GameState::Transition) {
        transitionElapsed += deltaSeconds;
        if (transitionElapsed >= Config::LevelTransitionSeconds) {
            transitionElapsed = Config::LevelTransitionSeconds;
            state = GameState::Playing;
        }
        return;
    }

    if (state != GameState::Playing) {
        return;
    }

    timer.update(deltaSeconds);
    if (timer.isExpired()) {
        setGameOver();
    }
}

void Game::render() {
    window.clear(sf::Color(24, 18, 9));

    if (state == GameState::Menu) {
        ui.drawMenu(window, menuSelection);
        window.display();
        return;
    }

    map.draw(window);
    player.draw(window);
    ui.drawHud(window, score, bestEndlessScore, visibleLevelNumber, timer.getTimeLeft(), mode);

    if (state == GameState::Transition) {
        ui.drawLevelTransition(window, visibleLevelNumber, transitionElapsed / Config::LevelTransitionSeconds, mode);
    } else if (state == GameState::Victory) {
        ui.drawVictory(window, finalVictory);
    } else if (state == GameState::GameOver) {
        ui.drawGameOver(window);
    }

    window.display();
}

void Game::startTraining() {
    mode = GameMode::Training;
    currentLevelIndex = 0;
    visibleLevelNumber = 1;
    score = 0;
    finalVictory = false;
    loadCurrentLevel();
}

void Game::startEndless() {
    mode = GameMode::Endless;
    currentLevelIndex = 0;
    visibleLevelNumber = 1;
    score = 0;
    endlessSeed = 1;
    finalVictory = false;
    loadCurrentLevel();
}

void Game::loadCurrentLevel(bool withTransition) {
    if (mode == GameMode::Training) {
        levelManager.loadTrainingLevel(currentLevelIndex);
    } else {
        levelManager.generateEndlessLevel(Config::GridWidth, Config::GridHeight, endlessSeed);
    }

    if (!map.loadFromLayout(levelManager.getCurrentLayout())) {
        std::cerr << "Level layout is invalid. The game returns to menu.\n";
        state = GameState::Menu;
        return;
    }

    player.reset(map.getStartCell(), map);
    timer.reset(Config::LevelTimeLimit);
    transitionElapsed = 0.f;
    state = withTransition ? GameState::Transition : GameState::Playing;
    finalVictory = false;
}

void Game::restartCurrentLevel() {
    if (mode == GameMode::Endless) {
        levelManager.generateEndlessLevel(Config::GridWidth, Config::GridHeight, endlessSeed);
    } else {
        levelManager.loadTrainingLevel(currentLevelIndex);
    }

    map.loadFromLayout(levelManager.getCurrentLayout());
    player.reset(map.getStartCell(), map);
    timer.reset(Config::LevelTimeLimit);
    transitionElapsed = 0.f;
    state = GameState::Transition;
    finalVictory = false;
}

void Game::goToNextLevelOrMenu() {
    if (finalVictory) {
        state = GameState::Menu;
        return;
    }

    if (mode == GameMode::Training) {
        ++currentLevelIndex;
        ++visibleLevelNumber;
    } else {
        ++visibleLevelNumber;
        ++endlessSeed;
    }

    loadCurrentLevel();
}

void Game::applyCellEffect() {
    CollectResult result = map.collectAt(player.getCell());

    if (result.points != 0) {
        score += result.points;
        if (mode == GameMode::Endless && score > bestEndlessScore) {
            bestEndlessScore = score;
        }
    }

    if (result.bonusSeconds > 0.f) {
        timer.addSeconds(result.bonusSeconds);
    }

    if (result.hitSpike) {
        setGameOver();
    }
}

void Game::setGameOver() {
    if (mode == GameMode::Endless && score > bestEndlessScore) {
        bestEndlessScore = score;
    }
    timer.stop();
    state = GameState::GameOver;
}

void Game::setVictory() {
    timer.stop();
    state = GameState::Victory;

    if (mode == GameMode::Training && currentLevelIndex + 1 >= levelManager.getTrainingLevelCount()) {
        finalVictory = true;
    } else {
        finalVictory = false;
    }
}

void Game::confirmMenuSelection() {
    if (menuSelection == 0) {
        startTraining();
    } else if (menuSelection == 1) {
        startEndless();
    } else {
        window.close();
    }
}
