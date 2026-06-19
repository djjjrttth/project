#pragma once

class GameTimer {
private:
    float timeLimit;
    float timeLeft;
    bool active;

public:
    explicit GameTimer(float limitSeconds = 45.f);

    void reset(float newLimitSeconds);
    void restart();
    void update(float deltaSeconds);
    void addSeconds(float seconds);
    void stop();

    bool isExpired() const;
    float getTimeLeft() const;
};
