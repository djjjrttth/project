#pragma once

#include <string>

namespace Config {
    inline constexpr int GridWidth = 31;
    inline constexpr int GridHeight = 23;
    inline constexpr int CellSize = 38;
    inline constexpr int TopPanelHeight = 72;

    inline constexpr unsigned int WindowWidth = GridWidth * CellSize;
    inline constexpr unsigned int WindowHeight = GridHeight * CellSize + TopPanelHeight;

    inline constexpr float PlayerSpeed = 700.f;
    inline constexpr float LevelTimeLimit = 45.f;
    inline constexpr float TimeBonusSeconds = 6.f;
    inline constexpr float LevelTransitionSeconds = 1.15f;

    inline constexpr float PlayerVisualSize = 36.f;
    inline constexpr float CoinVisualSize = 28.f;

    inline constexpr int CoinPoints = 10;
    inline constexpr int BonusPoints = 25;

    inline const std::string WindowTitle = "Backrooms runner";
    inline const std::string PlayerTexturePath = "assets/images/player.png";
    inline const std::string WallTexturePath = "assets/images/backrooms_1.png";
    inline const std::string CoinTexturePath = "assets/images/coin.png";
    inline const std::string MenuBackgroundPath = "assets/images/menu_background.png";
}

enum class GameMode {
    Training,
    Endless
};

enum class GameState {
    Menu,
    Transition,
    Playing,
    Victory,
    GameOver
};
