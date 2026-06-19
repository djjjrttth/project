#include "GameTimer.h"

#include <algorithm>

GameTimer::GameTimer(float limitSeconds)
    : timeLimit(limitSeconds), timeLeft(limitSeconds), active(false) {}

void GameTimer::reset(float newLimitSeconds) {
    timeLimit = newLimitSeconds;
    timeLeft = newLimitSeconds;
    active = true;
}

void GameTimer::restart() {
    timeLeft = timeLimit;
    active = true;
}

void GameTimer::update(float deltaSeconds) {
    if (!active) {
        return;
    }

    timeLeft -= deltaSeconds;
    if (timeLeft <= 0.f) {
        timeLeft = 0.f;
        active = false;
    }
}

void GameTimer::addSeconds(float seconds) {
    timeLeft = std::min(timeLeft + seconds, timeLimit + 20.f);
}

void GameTimer::stop() {
    active = false;
}

bool GameTimer::isExpired() const {
    return timeLeft <= 0.f;
}

float GameTimer::getTimeLeft() const {
    return timeLeft;
}
