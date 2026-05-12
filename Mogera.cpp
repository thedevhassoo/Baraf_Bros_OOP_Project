#include "Mogera.h"
#include "Botom.h"
#include "Player.h"
#include "Platform.h"
#include <cstdlib>
#include <iostream>
Mogera::Mogera() {
    setEnemyName("Mogera");
    setMaxHits(999);
    setScoreValue(5000);
    setGemDrop(0);

    maxBossHealth = 6;
    currentBossHealth = maxBossHealth;
    currentPhase = 1;
    phaseThreshold1 = 0.66f;
    phaseThreshold2 = 0.33f;
    spawnX = 0;
    spawnY = 0;

    //jumpSpeed = 300.0f;
    childCapacity = 20;
    childCount = 0;
    children = new Botom * [childCapacity];
    for (int i = 0; i < childCapacity; i++) children[i] = nullptr;

    spawnTimer = 0;
    spawnInterval = 2.0f;
    spawnAnimPlaying = false;
    spawnAnimTimer = 0;

    setHitboxSize(281, 273);
    setHitboxOffset(0, 0);


    healthBarBg.setSize(sf::Vector2f(200, 18));
    healthBarBg.setFillColor(sf::Color(60, 60, 60));
    healthBarBg.setOutlineColor(sf::Color::White);
    healthBarBg.setOutlineThickness(2);
    healthBarBg.setPosition(540, 690);

    healthBarFill.setSize(sf::Vector2f(200, 18));
    healthBarFill.setFillColor(sf::Color::Red);
    healthBarFill.setPosition(540, 690);
}

Mogera::~Mogera() {
    for (int i = 0; i < childCount; i++) {
        if (children[i]) delete children[i];
    }
    delete[] children;
}

void Mogera::hitBySnowballBoss(int power) {
    if (isEncased) return;
    currentBossHealth -= power;
    if (currentBossHealth <= 0) {
        currentBossHealth = 0;
        isEncased = true;
    }
    updateHealthBar();
    updatePhase();
}

bool Mogera::isDefeated() const { return currentBossHealth <= 0; }
int Mogera::getCurrentBossHealth() const { return currentBossHealth; }
int Mogera::getMaxBossHealth() const { return maxBossHealth; }

void Mogera::updateHealthBar() {
    float percent = (float)currentBossHealth / (float)maxBossHealth;
    healthBarFill.setSize(sf::Vector2f(200.0f * percent, 18.0f));
    if (currentPhase == 3) healthBarFill.setFillColor(sf::Color(255, 20, 20));
    else if (currentPhase == 2) healthBarFill.setFillColor(sf::Color(255, 120, 0));
}

void Mogera::updatePhase() {
    float percent = (float)currentBossHealth / (float)maxBossHealth;
    if (percent <= phaseThreshold2) {
        currentPhase = 3;
        spawnInterval = 0.6f;
    }
    else if (percent <= phaseThreshold1) {
        currentPhase = 2;
        spawnInterval = 1.2f;
    }
}

void Mogera::spawnChild(int levelColorVariant) {
    if (childCount >= childCapacity) {
        int newCap = childCapacity * 2;
        Botom** newArr = new Botom * [newCap];
        for (int i = 0; i < childCount; i++) newArr[i] = children[i];
        for (int i = childCount; i < newCap; i++) newArr[i] = nullptr;
        delete[] children;
        children = newArr;
        childCapacity = newCap;
    }

    Botom* child = new Botom();
    child->setPosition(posX + 40, posY + 124);
    child->setColorVariant(levelColorVariant);
    child->setHitboxSize(34, 30);
    child->setHitboxOffset(-17, -30);
    child->setGravity(980.0f);

    const char* paths[3] = {
        "./assets/enemies/botom_white.png",
        "./assets/enemies/botom_blue.png",
        "./assets/enemies/botom_black.png"
    };
    int frameSizes[3][2] = { {120,104},{121,111},{129,109} };
    int v = levelColorVariant; if (v < 0) v = 0; if (v > 2) v = 2;

    Spritesheet* sheet = new Spritesheet(3, paths[v], { frameSizes[v][0], frameSizes[v][1] });
    sheet->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(frameSizes[v][0], frameSizes[v][1]), 1);
    sheet->createAnim(1, 3, sf::Vector2i(frameSizes[v][0], 0), sf::Vector2i(frameSizes[v][0], frameSizes[v][1]), 8);
    sheet->createAnim(2, 3, sf::Vector2i(0, frameSizes[v][1]), sf::Vector2i(frameSizes[v][0], frameSizes[v][1]), 6);
    sheet->setAnimLoop(0, true);
    sheet->setAnimLoop(1, true);
    sheet->setAnimLoop(2, false);
    child->setEnemySpritesheet(sheet);
    child->configureEnemySprite(frameSizes[v][0], frameSizes[v][1], 70, 55);
    child->setHitboxOffset(0, -15);

    children[childCount++] = child;
}

Botom** Mogera::getChildren() { return children; }
int Mogera::getChildCount() { return childCount; }

void Mogera::removeChild(int index) {

    if (index < 0 || index >= childCount) return;
    if (children[index]) delete children[index];
    for (int i = index; i < childCount - 1; i++) children[i] = children[i + 1];
    children[childCount - 1] = nullptr;
    childCount--;
}

void Mogera::updateAI(float dt, Platform** platforms, int platformCount,
    Player* player1, Player* player2) {
    if (isEncased) return;
    // Face toward nearest player
    if (spawnY == 0) {
        spawnY = posY;
        spawnX = posX;
    }
    Player* target = player1;
    if (player2) {
        float d1 = posX - player1->getX(); if (d1 < 0) d1 = -d1;
        float d2 = posX - player2->getX(); if (d2 < 0) d2 = -d2;
        target = (d1 < d2) ? player1 : player2;
    }
    if (target) facingRight = (target->getX() > posX);

    // Spawn children on interval
    if (!spawnAnimPlaying) {
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval) {
            spawnAnimPlaying = true;
            spawnAnimTimer = 0.5f;
            spawnTimer = 0;
            if (hasEnemySpritesheet && enemySpritesheet) {
                enemySpritesheet->playAnim(1);
                enemyPrevAnim = 1;
            }
        }
    }

    if (spawnAnimPlaying) {
        spawnAnimTimer -= dt;
        if (spawnAnimTimer <= 0) {
            spawnAnimPlaying = false;
            spawnChild(0);
        }
    }

    // Apply gravity and land on platforms
  /*  if (!isGrounded) velocityY += gravity * dt;
    posX += velocityX * dt;
    posY += velocityY * dt;

    checkPlatformCollisions(platforms, platformCount);
    updateHitboxPosition();
    */
    // Stay in place — no movement
    velocityX = 0;
    velocityY = 0;

    // Apply gravity to keep him grounded on platform
    if (!isGrounded) {
        velocityY += 980.0f * dt;
        posY += velocityY * dt;
        checkPlatformCollisions(platforms, platformCount);
    }

    posX += velocityX * dt;
    updateHitboxPosition();
    if (posX < 150) posX = 150;
    if (posX > 1000) posX = 1000;

    // Spritesheet
    if (hasEnemySpritesheet && enemySpritesheet) {
        float hw = hitbox->getWidth();
        float hh = hitbox->getHeight();
        enemySpritesheet->setSpritePosition(posX + hw / 2.0f, posY + hh);
        if (!facingRight)
            enemySpritesheet->setSpriteScale(-1.0f, 1.0f);
        else
            enemySpritesheet->setSpriteScale(1.0f, 1.0f);
        enemySpritesheet->update(dt);
    }

    // Animation
    if (hasEnemySpritesheet) {
        if (isEncased) enemyCurrentAnim = 2;
        else if (spawnAnimPlaying) enemyCurrentAnim = 1;
        else enemyCurrentAnim = 0;

        if (enemyCurrentAnim != enemyPrevAnim) {
            enemySpritesheet->playAnim(enemyCurrentAnim);
            enemyPrevAnim = enemyCurrentAnim;
        }
    }

    // Update children
    for (int i = 0; i < childCount; i++) {
        if (children[i]) children[i]->updateCommon(dt, platforms, platformCount, player1, player2);
    }

    // Remove dead/rolled children
    for (int i = childCount - 1; i >= 0; i--) {
        if (children[i] && children[i]->getIsEncased() && children[i]->getIsRolling()) {
            removeChild(i);
        }
    }
}

void Mogera::draw(sf::RenderWindow& window) {

    if (hasEnemySpritesheet && enemySpritesheet) {
        enemySpritesheet->draw(window);
    }
    else {
        sf::RectangleShape body(sf::Vector2f(100, 90));
        body.setPosition(posX, posY);
        body.setFillColor(isEncased ? sf::Color::White : sf::Color(180, 40, 40));
        body.setOutlineColor(sf::Color(255, 80, 80));
        body.setOutlineThickness(3);
        window.draw(body);
    }
    for (int i = 0; i < childCount; i++) {
        if (children[i]) children[i]->draw(window);
    }
    if (!isEncased) {
        window.draw(healthBarBg);
        window.draw(healthBarFill);
    }
}

void Mogera::drawDebug(sf::RenderWindow& window) {
    Enemy::drawDebug(window);
    for (int i = 0; i < childCount; i++) {
        if (children[i]) children[i]->drawDebug(window);
    }
}