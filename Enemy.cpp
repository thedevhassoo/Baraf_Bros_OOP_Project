#include "Enemy.h"
#include "Player.h"
#include "Platform.h"
#include "Tile.h"
#include <cstring>
#include <iostream>

Enemy::Enemy()
    : maxHits(2), encaseProgress(0), isEncased(false), encaseTimer(0), encaseDuration(5.0f),
    isRolling(false), rollSpeed(300.0f),
    scoreValue(100), gemDrop(0),
    colorVariant(0) {
    speedMultiplier = 1.0f;
    enemyName[0] = '\0';

    enemySpritesheet = nullptr;
    hasEnemySpritesheet = false;
    enemyCurrentAnim = 0;
    enemyPrevAnim = -1;
}

Enemy::~Enemy() {
    if (enemySpritesheet) delete enemySpritesheet;
}
void Enemy::setEnemySpritesheet(Spritesheet* sheet) {
    enemySpritesheet = sheet;
    hasEnemySpritesheet = true;
}

void Enemy::configureEnemySprite(float frameW, float frameH, float targetW, float targetH) {
    if (!enemySpritesheet) return;
    enemySpritesheet->setSpriteOrigin(frameW / 2.0f, frameH);
    setHitboxSize(targetW, targetH);
    //setHitboxOffset(-targetW, -targetH);
    //setHitboxOffset(-5, -15);
}
void Enemy::hitBySnowball(int power) {
    if (isEncased && !isRolling) return;  // Already encased, can't double-encase

    encaseProgress += power;
    if (encaseProgress >= maxHits) {
        encaseProgress = maxHits;
        isEncased = true;
        encaseTimer = encaseDuration;
        velocityX = 0;
        velocityY = 0;
        isRolling = false;
    }
    if (hasEnemySpritesheet && enemySpritesheet) {
        enemyCurrentAnim = 2;   // Hurt/die anim (row 2 for Botom/Fooga/Tornado)
        enemySpritesheet->playAnim(enemyCurrentAnim);
        enemyPrevAnim = enemyCurrentAnim;
    }
}

bool Enemy::getIsEncased() const { return isEncased; }
bool Enemy::getIsPartiallyEncased() const { return (encaseProgress > 0 && !isEncased); }

void Enemy::shakeFree() {
    encaseProgress = 0;
    isEncased = false;
    isRolling = false;
    encaseTimer = 0;
}

void Enemy::updateEncaseTimer(float dt) {
    if (isEncased && !isRolling) {
        encaseTimer -= dt;
        if (encaseTimer <= 0) {
            shakeFree();
        }
    }
}

void Enemy::startRoll(float directionX) {
    if (!isEncased || isRolling) return;
    isRolling = true;
    if (directionX > 0) {
        velocityX = rollSpeed;
    }
    else if (directionX < 0) {
        velocityX = -rollSpeed;
    }
    else {
        velocityX = rollSpeed;
    }
    facingRight = (velocityX > 0);
}

void Enemy::stopRoll() {
    isRolling = false;
    velocityX = 0;
}

bool Enemy::getIsRolling() const { return isRolling; }

void Enemy::updateRollPhysics(float dt, Platform** platforms, int platformCount) {
    if (!isRolling) return;

    // Rolling enemy affected by gravity (spec 7.4)
    if (!isGrounded) {
        velocityY += gravity * dt;
    }

    posX += velocityX * dt;
    posY += velocityY * dt;
    //enemySpritesheet->setRotation(angle++);
    checkPlatformCollisions(platforms, platformCount);
    updateHitboxPosition();
}

void Enemy::checkPlatformCollisions(Platform** platforms, int count) {
    if (!hitbox || !platforms) return;

    isGrounded = false;
    float hw = hitbox->getWidth();
    float hh = hitbox->getHeight();
    float px = posX + hitboxOffsetX;
    float py = posY + hitboxOffsetY;
    float pBottom = py + hh;

    for (int i = 0; i < count; i++) {
        if (!platforms[i]) continue;

        Tile* tile = platforms[i]->getHeadTile();
        while (tile) {
            float tLeft = tile->getX();
            float tRight = tLeft + tile->getWidth();
            float tTop = tile->getY();
            float tBottom = tTop + tile->getHeight();

            if (px + hw > tLeft && px < tRight) {
                if (velocityY >= 0 && pBottom >= tTop && py < tTop) {
                    posY = tTop - hh - hitboxOffsetY;
                    velocityY = 0;
                    isGrounded = true;
                }
                else if (velocityY < 0 && py <= tBottom && pBottom > tBottom) {
                    posY = tBottom - hitboxOffsetY;
                    velocityY = 0;
                }
            }
            tile = tile->getNext();
        }
    }

    if (pBottom >= 720) {
        posY = 720 - hh - hitboxOffsetY;
        velocityY = 0;
        isGrounded = true;
    }

    updateHitboxPosition();
}

int Enemy::getMaxHits() const { return maxHits; }
int Enemy::getEncaseProgress() const { return encaseProgress; }
int Enemy::getScoreValue() const { return scoreValue; }
int Enemy::getGemDrop() const { return gemDrop; }
const char* Enemy::getEnemyName() const { return enemyName; }
int Enemy::getColorVariant() const { return colorVariant; }

void Enemy::setMaxHits(int h) { maxHits = h; }
void Enemy::setScoreValue(int s) { scoreValue = s; }
void Enemy::setGemDrop(int g) { gemDrop = g; }

void Enemy::setEnemyName(const char* name) {
    int i = 0;
    while (name[i] && i < 31) { enemyName[i] = name[i]; i++; }
    enemyName[i] = '\0';
}

void Enemy::setColorVariant(int v) {
    colorVariant = v;
}

void Enemy::setRollSpeed(float s) { rollSpeed = s; }
void Enemy::setEncaseDuration(float d) { encaseDuration = d; }





void Enemy::updateCommon(float dt, Platform** platforms, int platformCount,
    Player* player1, Player* player2) {
    if (isEncased) {
        updateEncaseTimer(dt);
        if (isRolling) {
            updateRollPhysics(dt, platforms, platformCount);
        }
        else {
            // Encased but not rolling: still affected by gravity
            if (!isGrounded) velocityY += gravity * dt;
            posY += velocityY * dt;
            checkPlatformCollisions(platforms, platformCount);
            updateHitboxPosition();
        }
        // Update spritesheet position during encased/roll
        if (hasEnemySpritesheet && enemySpritesheet) {
            float hw = hitbox->getWidth();
            float hh = hitbox->getHeight();
            enemySpritesheet->setSpritePosition(posX + hw / 2.0f, posY + hh);
            if (velocityX > 0)
                enemySpritesheet->setSpriteScale(1.0f, 1.0f);
            else if (velocityX < 0)
                enemySpritesheet->setSpriteScale(-1.0f, 1.0f);
            else if (!facingRight)
                enemySpritesheet->setSpriteScale(-1.0f, 1.0f);
            else
                enemySpritesheet->setSpriteScale(1.0f, 1.0f);
            enemySpritesheet->update(dt);
        }
        return;
    }

    // Normal AI
    updateAI(dt, platforms, platformCount, player1, player2);
    
    // Apply velocity and gravity
    if (!isGrounded) velocityY += gravity * dt;
    posX += velocityX * dt;
    posY += velocityY * dt;

    checkPlatformCollisions(platforms, platformCount);
    updateHitboxPosition();
    if (hasEnemySpritesheet && enemySpritesheet) {
        float hw =hitbox->getWidth();
        float hh = hitbox->getHeight();
        enemySpritesheet->setSpritePosition(posX + hw / 2.0f, posY + hh);
        if (!facingRight)
            enemySpritesheet->setSpriteScale(-1.0f, 1.0f);
        else
            enemySpritesheet->setSpriteScale(1.0f, 1.0f);
        enemySpritesheet->update(dt);
    }
}

void Enemy::update(float dt) {
    // Override in GameState to call updateCommon with parameters
}

void Enemy::drawDebug(sf::RenderWindow& window) {
    if (hitbox) {
        sf::Color color = isEncased ? sf::Color::Cyan : sf::Color::Red;
        hitbox->drawDebug(window, color);
    }
}