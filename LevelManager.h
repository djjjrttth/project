#pragma once

#include <string>
#include <vector>

class LevelManager {
private:
    std::vector<std::string> currentLayout;
    std::vector<std::string> makeFallbackLevel(int index) const;
    void addObjectsToGeneratedLevel(std::vector<std::string>& layout, int seed) const;

public:
    bool loadTrainingLevel(int index);
    void generateEndlessLevel(int width, int height, int seed);

    const std::vector<std::string>& getCurrentLayout() const;
    int getTrainingLevelCount() const;
};
