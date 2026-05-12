#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class Botom;

class Mogera : public Enemy {
private:
    int maxBossHealth;
    int currentBossHealth;
    int currentPhase;
    float phaseThreshold1;
    float phaseThreshold2;

    Botom** children;
    int childCount;
    int childCapacity;

    float spawnTimer;
    float spawnInterval;
    bool spawnAnimPlaying;
    float spawnAnimTimer;

    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBarFill;

    void updateHealthBar();
    void updatePhase();
    void spawnChild(int levelColorVariant);

    float spawnX, spawnY;

    int state;
    float stayTimer;

public:
    Mogera();
    ~Mogera();

    void updateAI(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2) override;
    void draw(sf::RenderWindow& window) override;

    void hitBySnowballBoss(int power);
    bool isDefeated() const;
    int getCurrentBossHealth() const;
    int getMaxBossHealth() const;

    Botom** getChildren();
    int getChildCount();
    void removeChild(int index);

    void drawDebug(sf::RenderWindow& window) override;
};