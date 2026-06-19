#include "UI.h"
#include "ResourcePath.h"

#include <iomanip>
#include <sstream>
#include <vector>

bool UI::loadDefaultFont() {
    const std::vector<std::string> candidates = {
        "assets/fonts/arial.ttf",
        "assets/fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    };

    for (const std::string& candidate : candidates) {
        const std::string path = findExistingPath(candidate);
        if (fileExists(path) && font.loadFromFile(path)) {
            fontLoaded = true;
            return true;
        }
    }

    fontLoaded = false;
    return false;
}

bool UI::loadMenuBackground(const std::string& path) {
    menuBackgroundLoaded = menuBackgroundTexture.loadFromFile(path);
    if (menuBackgroundLoaded) {
        menuBackgroundTexture.setSmooth(false);
    }
    return menuBackgroundLoaded;
}

bool UI::hasFont() const {
    return fontLoaded;
}

void UI::drawHud(sf::RenderWindow& window, int score, int bestScore, int level, float timeLeft, GameMode mode) const {
    sf::RectangleShape panel(sf::Vector2f(static_cast<float>(Config::WindowWidth), static_cast<float>(Config::TopPanelHeight)));
    panel.setPosition(0.f, 0.f);
    panel.setFillColor(sf::Color(25, 20, 10));
    panel.setOutlineThickness(-2.f);
    panel.setOutlineColor(sf::Color(80, 62, 18));
    window.draw(panel);

    if (!fontLoaded) {
        return;
    }

    std::ostringstream timeStream;
    timeStream << std::fixed << std::setprecision(0) << timeLeft;

    const std::string modeName = mode == GameMode::Training ? "Levels" : "Endless";
    sf::Text left = makeText("Mode: " + modeName + "   Level: " + std::to_string(level), 22, sf::Color(245, 233, 181));
    left.setPosition(16.f, 20.f);
    window.draw(left);

    std::string scoreText = "Score: " + std::to_string(score);
    if (mode == GameMode::Endless) {
        scoreText += "   Best: " + std::to_string(bestScore);
    }
    sf::Text center = makeText(scoreText, 24, sf::Color(255, 214, 59));
    sf::FloatRect centerBounds = center.getLocalBounds();
    center.setOrigin(centerBounds.left + centerBounds.width / 2.f, centerBounds.top + centerBounds.height / 2.f);
    center.setPosition(Config::WindowWidth / 2.f, Config::TopPanelHeight / 2.f);
    window.draw(center);

    sf::Text right = makeText("Time: " + timeStream.str(), 22, sf::Color(255, 245, 190));
    sf::FloatRect rightBounds = right.getLocalBounds();
    right.setOrigin(rightBounds.left + rightBounds.width, 0.f);
    right.setPosition(Config::WindowWidth - 18.f, 20.f);
    window.draw(right);
}

void UI::drawMenu(sf::RenderWindow& window, int selectedIndex) const {
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(Config::WindowWidth), static_cast<float>(Config::WindowHeight)));
    background.setFillColor(sf::Color(46, 39, 13));
    window.draw(background);

    if (menuBackgroundLoaded) {
        sf::Sprite bg;
        bg.setTexture(menuBackgroundTexture);
        const sf::Vector2u size = menuBackgroundTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            bg.setScale(
                static_cast<float>(Config::WindowWidth) / static_cast<float>(size.x),
                static_cast<float>(Config::WindowHeight) / static_cast<float>(size.y)
            );
        }
        bg.setPosition(0.f, 0.f);
        window.draw(bg);
    }

    sf::RectangleShape tint(sf::Vector2f(static_cast<float>(Config::WindowWidth), static_cast<float>(Config::WindowHeight)));
    tint.setFillColor(sf::Color(0, 0, 0, 65));
    window.draw(tint);

    const float panelX = 84.f;
    const float panelY = 286.f;
    const float panelWidth = 430.f;
    const float buttonHeight = 66.f;
    const float gap = 16.f;

    const std::vector<std::string> items = {
        "LEVEL MODE",
        "ENDLESS MODE",
        "EXIT"
    };

    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        sf::RectangleShape shadow(sf::Vector2f(panelWidth, buttonHeight));
        shadow.setPosition(panelX + 6.f, panelY + i * (buttonHeight + gap) + 6.f);
        shadow.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(shadow);

        sf::RectangleShape button(sf::Vector2f(panelWidth, buttonHeight));
        button.setPosition(panelX, panelY + i * (buttonHeight + gap));
        const bool selected = i == selectedIndex;
        button.setFillColor(selected ? sf::Color(230, 196, 34) : sf::Color(15, 13, 10, 225));
        button.setOutlineThickness(3.f);
        button.setOutlineColor(selected ? sf::Color(255, 242, 168) : sf::Color(68, 56, 18));
        window.draw(button);

        if (fontLoaded) {
            sf::Text text = makeText(items[static_cast<std::size_t>(i)], 30, selected ? sf::Color(18, 14, 5) : sf::Color(240, 233, 185));
            text.setPosition(panelX + 54.f, panelY + i * (buttonHeight + gap) + 14.f);
            window.draw(text);

            if (selected) {
                sf::Text arrow = makeText(">", 34, sf::Color(18, 14, 5));
                arrow.setPosition(panelX + 18.f, panelY + i * (buttonHeight + gap) + 10.f);
                window.draw(arrow);
            }
        }
    }

    if (!fontLoaded) {
        return;
    }

    drawCenteredText(window, "TOMB OF THE MASK", 104.f, 54, sf::Color(255, 235, 150));
    drawCenteredText(window, "Student Edition", 154.f, 28, sf::Color(255, 247, 215));
    drawCenteredText(window, "W / S or arrows - select   Enter - confirm", 694.f, 22, sf::Color(245, 238, 200));
    drawCenteredText(window, "In game: WASD / arrows - move one cell   R - restart   Esc - menu", 732.f, 18, sf::Color(220, 210, 170));
}

void UI::drawLevelTransition(sf::RenderWindow& window, int level, float progress, GameMode mode) const {
    if (progress < 0.f) {
        progress = 0.f;
    }
    if (progress > 1.f) {
        progress = 1.f;
    }

    const float opening = progress;
    const float halfWidth = static_cast<float>(Config::WindowWidth) / 2.f;
    const float doorWidth = halfWidth * (1.f - opening);
    const float doorY = static_cast<float>(Config::TopPanelHeight);
    const float doorHeight = static_cast<float>(Config::WindowHeight - Config::TopPanelHeight);

    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(Config::WindowWidth), doorHeight));
    overlay.setPosition(0.f, doorY);
    overlay.setFillColor(sf::Color(0, 0, 0, static_cast<unsigned char>(140 - progress * 70.f)));
    window.draw(overlay);

    sf::Color doorColor(79, 63, 19, 235);
    sf::Color frameColor(32, 24, 8, 255);

    sf::RectangleShape leftDoor(sf::Vector2f(doorWidth, doorHeight));
    leftDoor.setPosition(0.f, doorY);
    leftDoor.setFillColor(doorColor);
    leftDoor.setOutlineThickness(-4.f);
    leftDoor.setOutlineColor(frameColor);
    window.draw(leftDoor);

    sf::RectangleShape rightDoor(sf::Vector2f(doorWidth, doorHeight));
    rightDoor.setPosition(static_cast<float>(Config::WindowWidth) - doorWidth, doorY);
    rightDoor.setFillColor(doorColor);
    rightDoor.setOutlineThickness(-4.f);
    rightDoor.setOutlineColor(frameColor);
    window.draw(rightDoor);

    const float handleRadius = 6.f;
    if (doorWidth > 20.f) {
        sf::CircleShape leftHandle(handleRadius);
        leftHandle.setFillColor(sf::Color(220, 190, 80));
        leftHandle.setPosition(halfWidth - doorWidth * 0.25f - handleRadius, doorY + doorHeight / 2.f);
        window.draw(leftHandle);

        sf::CircleShape rightHandle(handleRadius);
        rightHandle.setFillColor(sf::Color(220, 190, 80));
        rightHandle.setPosition(halfWidth + doorWidth * 0.25f - handleRadius, doorY + doorHeight / 2.f);
        window.draw(rightHandle);
    }

    if (!fontLoaded) {
        return;
    }

    const std::string modeName = mode == GameMode::Training ? "LEVEL MODE" : "ENDLESS MODE";
    drawCenteredText(window, modeName, 300.f, 24, sf::Color(255, 244, 196));
    drawCenteredText(window, "LEVEL " + std::to_string(level), 350.f, 56, sf::Color(255, 214, 68));
    drawCenteredText(window, "The door is opening...", 405.f, 24, sf::Color::White);
}

void UI::drawVictory(sf::RenderWindow& window, bool finalLevel) const {
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(Config::WindowWidth), static_cast<float>(Config::WindowHeight)));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    if (!fontLoaded) {
        return;
    }

    drawCenteredText(window, finalLevel ? "FULL VICTORY!" : "LEVEL COMPLETE!", 260.f, 48, sf::Color(112, 255, 130));
    drawCenteredText(window, finalLevel ? "Enter - return to menu" : "Enter - next level", 328.f, 28, sf::Color::White);
    drawCenteredText(window, "Esc - menu", 370.f, 20, sf::Color(220, 220, 220));
}

void UI::drawGameOver(sf::RenderWindow& window) const {
    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(Config::WindowWidth), static_cast<float>(Config::WindowHeight)));
    overlay.setFillColor(sf::Color(0, 0, 0, 155));
    window.draw(overlay);

    if (!fontLoaded) {
        return;
    }

    drawCenteredText(window, "TIME IS OVER", 260.f, 48, sf::Color(255, 80, 95));
    drawCenteredText(window, "R - retry", 328.f, 28, sf::Color::White);
    drawCenteredText(window, "Esc - menu", 370.f, 20, sf::Color(220, 220, 220));
}

sf::Text UI::makeText(const std::string& value, unsigned int size, sf::Color color) const {
    sf::Text text;
    text.setFont(font);
    text.setString(value);
    text.setCharacterSize(size);
    text.setFillColor(color);
    return text;
}

void UI::drawCenteredText(sf::RenderWindow& window, const std::string& value, float y, unsigned int size, sf::Color color) const {
    sf::Text text = makeText(value, size, color);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(Config::WindowWidth / 2.f, y);
    window.draw(text);
}
