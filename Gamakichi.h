#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class ArtilleryRocket;

class Gamakichi : public Enemy {
private:
    int maxBossHealth, currentBossHealth;
    int currentPhase;
    float phaseThreshold1, phaseThreshold2;

    ArtilleryRocket** rockets;
    int rocketCount, rocketCapacity;
    float rocketTimer, rocketInterval;

    float moveTimer, moveInterval;
    int moveDirection;
    float bossSpeed;

    sf::RectangleShape healthBarBg, healthBarFill;

    void updateHealthBar();
    void updatePhase();
    void fireRockets(Player* player1, Player* player2);

public:
    Gamakichi();
    ~Gamakichi();

    void updateAI(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2) override;
    void draw(sf::RenderWindow& window) override;

    void hitBySnowballBoss(int power);
    bool isDefeated() const;
    int getCurrentBossHealth() const;
    int getMaxBossHealth() const;
    int getCurrentPhase() const;

    ArtilleryRocket** getRockets();
    int getRocketCount();
    void removeRocket(int index);

    void drawDebug(sf::RenderWindow& window) override;
};