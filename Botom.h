#pragma once
#include "Enemy.h"

class Botom : public Enemy {
private:
    float aiTimer;
    float aiInterval;
    int moveDirection;      // -1 left, 0 idle, 1 right
    float walkSpeed;
    //bool fallingOff;
    void pickNewDirection();

public:
    Botom();
    ~Botom();
    //void setupAnimations(float frameW, float frameH);
    void updateAI(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2) override;
    void draw(sf::RenderWindow& window) override;
};