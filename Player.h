#pragma once
#include "Character2D.h"
#include "Spritesheet.h"
#include "HitBox.h"
#include <SFML/Audio.hpp>

class Platform;

class Player : public Character2D {
private:
    int playerIndex;
    int lives;
    int snowballPower;
    float snowballDistance;
    float moveSpeed;
    float jumpForce;

    int keyLeft;
    int keyRight;
    int keyJump;
    int keyThrow;

    float throwCooldown;
    float throwTimer;
    bool canThrowSnowball;

    bool speedBoosted;
    float speedBoostTimer;
    bool snowballPowered;
    bool distanceIncreased;
    bool balloonMode;
    float balloonTimer;

    Spritesheet* spritesheet;
    int currentAnim;
    enum AnimState { ANIM_IDLE, ANIM_WALK, ANIM_JUMP };
    AnimState animState;

    sf::SoundBuffer jumpSoundBuffer;
    sf::Sound jumpSound;
    bool hasJumpSound;

    sf::SoundBuffer throwSoundBuffer;
    sf::Sound throwSound;
    bool hasThrowSound;

    void updateAnimation(float dt);
    

public:
    bool isDying;
    float dyingTimer;
    float hitTimer;
    Player();
    ~Player();

    void init(int playerIdx, float startX, float startY);
    void setKeys(int left, int right, int jump, int throwKey);
    void setSpritesheet(Spritesheet* sheet);
    void setJumpSound(const char* path);
    void setThrowSound(const char* path);

    int getPlayerIndex() const;
    int getLives() const;
    float getMoveSpeed() const;
    bool isSpeedBoosted() const;
    bool isBalloonMode() const;
    bool canThrow() const;

    void setLives(int l);

    void moveLeft();
    void moveRight();
    void stopHorizontal();
    void jump();
    void throwSnowball();
    void resetThrowCooldown();

    void takeDamage();
    void addLife();

    void applySpeedBoost(float duration);
    void applySnowballPower();
    void applyDistanceIncrease();
    void applyBalloonMode(float duration);
    void updatePowerUps(float dt);
    void clearPowerUps();

    void checkPlatformCollisions(Platform** platforms, int count);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void drawDebug(sf::RenderWindow& window) override;

};