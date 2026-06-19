#pragma once

#include "Config.h"

#include <SFML/Graphics.hpp>
#include <string>

class UI {
private:
    sf::Font font;
    bool fontLoaded = false;

    sf::Texture menuBackgroundTexture;
    bool menuBackgroundLoaded = false;

    sf::Text makeText(const std::string& value, unsigned int size, sf::Color color) const;
    void drawCenteredText(sf::RenderWindow& window, const std::string& value, float y, unsigned int size, sf::Color color) const;

public:
    bool loadDefaultFont();
    bool loadMenuBackground(const std::string& path);
    bool hasFont() const;

    void drawHud(sf::RenderWindow& window, int score, int bestScore, int level, float timeLeft, GameMode mode) const;
    void drawMenu(sf::RenderWindow& window, int selectedIndex) const;
    void drawLevelTransition(sf::RenderWindow& window, int level, float progress, GameMode mode) const;
    void drawVictory(sf::RenderWindow& window, bool finalLevel) const;
    void drawGameOver(sf::RenderWindow& window) const;
};
