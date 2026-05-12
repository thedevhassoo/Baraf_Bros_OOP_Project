#pragma once
#include "Character2D.h"
#include "Spritesheet.h"
#include <SFML/Graphics.hpp>

class Player;
class Platform;

class Enemy : public Character2D {
protected:
    // Encase system
    int maxHits;                // Hits needed to fully encase
    int encaseProgress;         // Current hits received
    bool isEncased;             // Fully encased = rollable snowball
    float encaseTimer;          // Time until shakes free
    float encaseDuration;       // How long encased before auto-free
    
    // Roll state
    bool isRolling;
    float rollSpeed;

    // Rewards
    int scoreValue;
    int gemDrop;

    // Identity
    char enemyName[32];
    int colorVariant;           // 0=Red, 1=Green, 2=Blue

    Spritesheet* enemySpritesheet;
    bool hasEnemySpritesheet;
    int enemyCurrentAnim;
    int enemyPrevAnim;

public:
    Enemy();
    virtual ~Enemy();
    float speedMultiplier;
    // Pure virtual — each enemy type defines its own AI
    virtual void updateAI(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2) = 0;
    virtual void draw(sf::RenderWindow& window) override = 0;

    // Common update (physics + encase timer + AI)
    void update(float dt) override;
    void updateCommon(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2);
    void setEnemySpritesheet(Spritesheet* sheet);
    void configureEnemySprite(float frameW, float frameH, float targetW, float targetH);
    // Encase system
    void hitBySnowball(int power);
    bool getIsEncased() const;
    bool getIsPartiallyEncased() const;
    void shakeFree();
    void updateEncaseTimer(float dt);

    // Roll system
    void startRoll(float directionX);
    void stopRoll();
    bool getIsRolling() const;
    void updateRollPhysics(float dt, Platform** platforms, int platformCount);

    // Platform collision for enemies
    void checkPlatformCollisions(Platform** platforms, int count);

    // Getters
    int getMaxHits() const;
    int getEncaseProgress() const;
    int getScoreValue() const;
    int getGemDrop() const;
    const char* getEnemyName() const;
    int getColorVariant() const;

    // Setters
    void setMaxHits(int h);
    void setScoreValue(int s);
    void setGemDrop(int g);
    void setEnemyName(const char* name);
    void setColorVariant(int v);
    void setRollSpeed(float s);
    void setEncaseDuration(float d);



    // Debug
    void drawDebug(sf::RenderWindow& window) override;
};