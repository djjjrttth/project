#include "LevelManager.h"
#include "ResourcePath.h"

#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

namespace {
    bool isInside(const std::vector<std::string>& layout, int x, int y) {
        return y >= 0 && y < static_cast<int>(layout.size()) &&
               x >= 0 && !layout.empty() && x < static_cast<int>(layout[y].size());
    }

    bool isWalkableTile(char tile) {
        return tile != '#';
    }

    sf::Vector2i findTile(const std::vector<std::string>& layout, char tile, sf::Vector2i fallback) {
        for (int y = 0; y < static_cast<int>(layout.size()); ++y) {
            for (int x = 0; x < static_cast<int>(layout[y].size()); ++x) {
                if (layout[y][x] == tile) {
                    return {x, y};
                }
            }
        }
        return fallback;
    }

    bool hasPathToExit(const std::vector<std::string>& layout) {
        if (layout.empty() || layout.front().empty()) {
            return false;
        }

        const int width = static_cast<int>(layout.front().size());
        const int height = static_cast<int>(layout.size());
        const sf::Vector2i start = findTile(layout, 'P', {1, 1});
        const sf::Vector2i exitCell = findTile(layout, 'E', {width - 2, height - 2});

        if (!isInside(layout, start.x, start.y) || !isInside(layout, exitCell.x, exitCell.y)) {
            return false;
        }

        std::vector<std::vector<bool>> visited(static_cast<std::size_t>(height), std::vector<bool>(static_cast<std::size_t>(width), false));
        std::queue<sf::Vector2i> queue;
        queue.push(start);
        visited[start.y][start.x] = true;

        const std::vector<sf::Vector2i> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        while (!queue.empty()) {
            sf::Vector2i current = queue.front();
            queue.pop();

            if (current.x == exitCell.x && current.y == exitCell.y) {
                return true;
            }

            for (const sf::Vector2i& direction : directions) {
                sf::Vector2i next{current.x + direction.x, current.y + direction.y};
                if (!isInside(layout, next.x, next.y)) {
                    continue;
                }
                if (visited[next.y][next.x]) {
                    continue;
                }
                if (!isWalkableTile(layout[next.y][next.x])) {
                    continue;
                }

                visited[next.y][next.x] = true;
                queue.push(next);
            }
        }

        return false;
    }

    struct Frontier {
        int wallX;
        int wallY;
        int nextX;
        int nextY;
    };

    void addFrontier(std::vector<Frontier>& frontier, const std::vector<std::string>& layout, int x, int y) {
        static const int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};
        for (const auto& direction : directions) {
            const int nextX = x + direction[0];
            const int nextY = y + direction[1];
            const int wallX = x + direction[0] / 2;
            const int wallY = y + direction[1] / 2;

            if (isInside(layout, nextX, nextY) && layout[nextY][nextX] == '#') {
                frontier.push_back({wallX, wallY, nextX, nextY});
            }
        }
    }

    std::vector<std::string> generatePrimMazeBase(int width, int height, int seed) {
        if (width % 2 == 0) {
            --width;
        }
        if (height % 2 == 0) {
            --height;
        }

        std::vector<std::string> layout(static_cast<std::size_t>(height), std::string(static_cast<std::size_t>(width), '#'));
        std::mt19937 rng(static_cast<unsigned int>(seed));

        layout[1][1] = ' ';
        std::vector<Frontier> frontier;
        addFrontier(frontier, layout, 1, 1);

        while (!frontier.empty()) {
            std::uniform_int_distribution<int> pick(0, static_cast<int>(frontier.size()) - 1);
            const int index = pick(rng);
            Frontier chosen = frontier[static_cast<std::size_t>(index)];
            frontier[static_cast<std::size_t>(index)] = frontier.back();
            frontier.pop_back();

            if (layout[chosen.nextY][chosen.nextX] == '#') {
                layout[chosen.wallY][chosen.wallX] = ' ';
                layout[chosen.nextY][chosen.nextX] = ' ';
                addFrontier(frontier, layout, chosen.nextX, chosen.nextY);
            }
        }

        return layout;
    }

    void addExtraOpenings(std::vector<std::string>& layout, std::mt19937& rng) {
        const int width = static_cast<int>(layout.front().size());
        const int height = static_cast<int>(layout.size());

        std::uniform_int_distribution<int> xDist(1, width - 2);
        std::uniform_int_distribution<int> yDist(1, height - 2);

        for (int i = 0; i < 28; ++i) {
            int x = xDist(rng);
            int y = yDist(rng);
            if (layout[y][x] != '#') {
                continue;
            }

            int openNeighbors = 0;
            if (isInside(layout, x + 1, y) && layout[y][x + 1] != '#') ++openNeighbors;
            if (isInside(layout, x - 1, y) && layout[y][x - 1] != '#') ++openNeighbors;
            if (isInside(layout, x, y + 1) && layout[y + 1][x] != '#') ++openNeighbors;
            if (isInside(layout, x, y - 1) && layout[y - 1][x] != '#') ++openNeighbors;

            if (openNeighbors >= 2) {
                layout[y][x] = ' ';
            }
        }

        for (int room = 0; room < 5; ++room) {
            int x = 2 + xDist(rng) % std::max(2, width - 4);
            int y = 2 + yDist(rng) % std::max(2, height - 4);
            for (int dy = 0; dy < 2; ++dy) {
                for (int dx = 0; dx < 2; ++dx) {
                    if (isInside(layout, x + dx, y + dy)) {
                        layout[y + dy][x + dx] = ' ';
                    }
                }
            }
        }
    }

    void placeStartAndExit(std::vector<std::string>& layout) {
        const int width = static_cast<int>(layout.front().size());
        const int height = static_cast<int>(layout.size());

        layout[1][1] = 'P';
        layout[height - 2][width - 2] = 'E';
    }

    void removeSpikes(std::vector<std::string>& layout) {
        for (std::string& row : layout) {
            for (char& tile : row) {
                if (tile == 'S') {
                    tile = ' ';
                }
            }
        }
    }

    std::vector<std::string> makeEndlessPrimLevel(int width, int height, int seed) {
        for (int attempt = 0; attempt < 40; ++attempt) {
            std::mt19937 rng(static_cast<unsigned int>(seed * 101 + attempt * 53 + 9));
            std::vector<std::string> layout = generatePrimMazeBase(width, height, seed * 997 + attempt * 13 + 17);
            addExtraOpenings(layout, rng);
            placeStartAndExit(layout);

            if (hasPathToExit(layout)) {
                return layout;
            }
        }

        return {
            "###############################",
            "#P      #       #           # #",
            "###### ## ##### # ##### ### # #",
            "#    #    #   #   #   # #   # #",
            "# ## ###### # ##### # # # ### #",
            "# ##        #       # # #     #",
            "# ######### ######### # ##### #",
            "#   #     #     #     #     # #",
            "### # ### ##### # ######### # #",
            "#   # #       # #         # # #",
            "# ### # ##### # ####### ### # #",
            "# #   # #   # #       #     # #",
            "# # ### # # # ####### ##### # #",
            "# # #   # # #     #   #   # # #",
            "# # # ### # ##### # ### # # # #",
            "#   #   # #     # #     # #   #",
            "####### # ##### # ####### #####",
            "#       #     # #         #   #",
            "# ########### # ########### # #",
            "#           # #           # # #",
            "########### # ########### # # #",
            "#           #             #  E#",
            "###############################"
        };
    }
}

bool LevelManager::loadTrainingLevel(int index) {
    const std::string filename = "levels/level" + std::to_string(index + 1) + ".txt";
    const std::string path = findExistingPath(filename);

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open " << filename << ". Fallback level loaded.\n";
        currentLayout = makeFallbackLevel(index);
        removeSpikes(currentLayout);
        return false;
    }

    currentLayout.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            currentLayout.push_back(line);
        }
    }

    if (currentLayout.empty()) {
        currentLayout = makeFallbackLevel(index);
        removeSpikes(currentLayout);
        return false;
    }

    removeSpikes(currentLayout);
    if (!hasPathToExit(currentLayout)) {
        currentLayout = makeFallbackLevel(index);
        removeSpikes(currentLayout);
        return false;
    }

    return true;
}

void LevelManager::generateEndlessLevel(int width, int height, int seed) {
    std::vector<std::string> layout = makeEndlessPrimLevel(width, height, seed);
    addObjectsToGeneratedLevel(layout, seed);
    currentLayout = layout;
}

const std::vector<std::string>& LevelManager::getCurrentLayout() const {
    return currentLayout;
}

int LevelManager::getTrainingLevelCount() const {
    return 3;
}

void LevelManager::addObjectsToGeneratedLevel(std::vector<std::string>& layout, int seed) const {
    removeSpikes(layout);

    std::mt19937 generator(static_cast<unsigned int>(seed * 37 + 17));
    std::uniform_int_distribution<int> chance(0, 99);

    for (int y = 1; y < static_cast<int>(layout.size()) - 1; ++y) {
        for (int x = 1; x < static_cast<int>(layout[y].size()) - 1; ++x) {
            char& tile = layout[y][x];
            if (tile != ' ') {
                continue;
            }
            if (x + y < 5) {
                continue;
            }
            if (x > static_cast<int>(layout[y].size()) - 5 && y > static_cast<int>(layout.size()) - 5) {
                continue;
            }

            const int value = chance(generator);
            if (value < 22) {
                tile = '.';
            } else if (value >= 96) {
                tile = 'B';
            }
        }
    }
}

std::vector<std::string> LevelManager::makeFallbackLevel(int index) const {
    (void)index;
    return {
        "###############################",
        "#P  .   #       #     .     . #",
        "### ### # ##### # ####### ### #",
        "#   #   # #   # #       # #   #",
        "# ### ### # # # ####### # # ###",
        "#     .   # # #     . # # #   #",
        "##### ##### # ##### ### # ### #",
        "#   #     # #     #     #   # #",
        "# # ##### # ##### ######### # #",
        "# #     # #     #         # # #",
        "# ##### # ##### ####### ### # #",
        "#     # # .   #       #     # #",
        "##### # ##### ####### ##### # #",
        "#     #     #     #   #   # # #",
        "# ######### ##### # ### # # # #",
        "#       . #     # #     # #   #",
        "####### # ##### # ####### #####",
        "#       #     # #         #   #",
        "# ########### # ########### # #",
        "#           # #           # # #",
        "########### # ########### # # #",
        "#       B   #       .     #  E#",
        "###############################"
    };
}
