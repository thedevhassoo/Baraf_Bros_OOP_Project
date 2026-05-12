#include "Gamakichi.h"
#include "ArtilleryRocket.h"
#include "Player.h"
#include "Platform.h"
#include <cstdlib>

Gamakichi::Gamakichi() {
    setEnemyName("Gamakichi");
    setMaxHits(999);
    setScoreValue(100000);
    setGemDrop(0);

    maxBossHealth = 12;
    currentBossHealth = maxBossHealth;

    currentPhase = 1;
    phaseThreshold1 = 0.66f;
    phaseThreshold2 = 0.33f;

    rocketCapacity = 15;
    rocketCount = 0;
    rockets = new ArtilleryRocket * [rocketCapacity];
    for (int i = 0; i < rocketCapacity; i++) rockets[i] = nullptr;
    rocketTimer = 0;
    rocketInterval = 1.8f;

    moveTimer = 0;
    moveInterval = 3.5f;
    moveDirection = 0;
    bossSpeed = 45.0f;

    setHitboxSize(110, 90);
    setHitboxOffset(20, 10);
    gravity = 980.0f;

    healthBarBg.setSize(sf::Vector2f(280, 20));
    healthBarBg.setFillColor(sf::Color(50, 50, 50));
    healthBarBg.setOutlineColor(sf::Color::White);
    healthBarBg.setOutlineThickness(2);
    healthBarBg.setPosition(500, 690);

    healthBarFill.setSize(sf::Vector2f(280, 20));
    healthBarFill.setFillColor(sf::Color(200, 0, 0));
    healthBarFill.setPosition(500, 690);
}

Gamakichi::~Gamakichi() {
    for (int i = 0; i < rocketCount; i++) delete rockets[i];
    delete[] rockets;
}

void Gamakichi::hitBySnowballBoss(int power) {
    if (isEncased) return;
    currentBossHealth -= power;
    if (currentBossHealth <= 0) {
        currentBossHealth = 0;
        isEncased = true;
    }
    updateHealthBar();
    updatePhase();
}

bool Gamakichi::isDefeated() const { return currentBossHealth <= 0; }
int Gamakichi::getCurrentBossHealth() const { return currentBossHealth; }
int Gamakichi::getMaxBossHealth() const { return maxBossHealth; }
int Gamakichi::getCurrentPhase() const { return currentPhase; }

void Gamakichi::updateHealthBar() {
    float percent = (float)currentBossHealth / (float)maxBossHealth;
    healthBarFill.setSize(sf::Vector2f(280.0f * percent, 20.0f));
    if (currentPhase == 3) healthBarFill.setFillColor(sf::Color(255, 20, 20));
    else if (currentPhase == 2) healthBarFill.setFillColor(sf::Color(255, 120, 0));
}

void Gamakichi::updatePhase() {
    float percent = (float)currentBossHealth / (float)maxBossHealth;
    if (percent <= phaseThreshold2) {
        currentPhase = 3;
        rocketInterval = 0.4f;   // Phase 3: fire every 0.4 seconds
    }
    else if (percent <= phaseThreshold1) {
        currentPhase = 2;
        rocketInterval = 0.7f;   // Phase 2: fire every 0.7 seconds
    }
    else {
        rocketInterval = 1.0f;   // Phase 1: fire every 1 second
    }
}

void Gamakichi::fireRockets(Player* player1, Player* player2) {
    if (rocketCount >= rocketCapacity) return;

    Player* target = player1;
    if (player2 && rand() % 2 == 0) target = player2;
    if (!target) return;

    // Only fire 1 bomb per call, timer will call again for next
    ArtilleryRocket* rocket = new ArtilleryRocket();
    float tx = target->getX() + 16;
    float ty = target->getY();
    rocket->launch(posX + 65, posY + 50, tx, ty, 400.0f);
    rockets[rocketCount++] = rocket;
}

ArtilleryRocket** Gamakichi::getRockets() { return rockets; }
int Gamakichi::getRocketCount() { return rocketCount; }

void Gamakichi::removeRocket(int index) {
    if (index < 0 || index >= rocketCount) return;
    delete rockets[index];
    for (int i = index; i < rocketCount - 1; i++) rockets[i] = rockets[i + 1];
    rockets[rocketCount - 1] = nullptr;
    rocketCount--;
}

void Gamakichi::updateAI(float dt, Platform** platforms, int platformCount,
    Player* player1, Player* player2) {
    if (isEncased) {
        enemyCurrentAnim = 2;
        if (enemyCurrentAnim != enemyPrevAnim) {
            enemySpritesheet->playAnim(enemyCurrentAnim);
            enemyPrevAnim = enemyCurrentAnim;
        }
        enemySpritesheet->update(dt);
        return;
    }
    checkPlatformCollisions(platforms, platformCount);
    moveTimer += dt;
    if (moveTimer >= moveInterval) {
        moveTimer = 0;
        moveDirection = (rand() % 3 == 0) ? ((rand() % 2) ? 1 : -1) : 0;
        moveInterval = 3.0f + (rand() % 200) / 100.0f;
    }
    velocityX = moveDirection * bossSpeed;
    velocityY = gravity;
    posY += velocityY * dt;
    facingRight = (moveDirection > 0);

    if (posX < 80) posX = 80;
    if (posX > 1090) posX = 1090;
    if (posY < 80) posY = 80;
    if (posY > 400) posY = 400;

    rocketTimer += dt;
    if (rocketTimer >= rocketInterval) {
        rocketTimer = 0;
        fireRockets(player1, player2);
    }

    for (int i = 0; i < rocketCount; i++) {
        if (rockets[i]) {
            rockets[i]->update(dt, platforms, platformCount);
            if (!rockets[i]->isActive()) {
                removeRocket(i);
                i--;
            }
        }
    }

    if (hasEnemySpritesheet && enemySpritesheet) {
        float hw = getHitbox()->getWidth();
        float hh = getHitbox()->getHeight();

        // Position at bottom-center of hitbox
        enemySpritesheet->setSpritePosition(posX + hw / 2.0f, posY + hh);

        // Face toward nearest player
        Player* target = player1;
        if (player2) {
            float d1 = posX - player1->getX();
            float d2 = posX - player2->getX();
            if (d1 < 0) d1 = -d1;
            if (d2 < 0) d2 = -d2;
            target = (d1 < d2) ? player1 : player2;
        }

        if (target && target->getX() < posX) {
            enemySpritesheet->setSpriteScale(-1.0f, 1.0f);
        }
        else {
            enemySpritesheet->setSpriteScale(1.0f, 1.0f);
        }

        enemySpritesheet->update(dt);

        // Animation
        if (isEncased) enemyCurrentAnim = 2;
        else enemyCurrentAnim = (rocketTimer > rocketInterval - 0.3f) ? 0 : 1;

        if (enemyCurrentAnim != enemyPrevAnim) {
            enemySpritesheet->playAnim(enemyCurrentAnim);
            enemyPrevAnim = enemyCurrentAnim;
        }
    }

}

void Gamakichi::draw(sf::RenderWindow& window) {
    for (int i = 0; i < rocketCount; i++) {
        if (rockets[i] && rockets[i]->isActive()) rockets[i]->draw(window);
    }

    if (hasEnemySpritesheet && enemySpritesheet) {
        enemySpritesheet->draw(window);
    }
    else {
        sf::RectangleShape body(sf::Vector2f(130, 100));
        body.setPosition(posX, posY);
        body.setFillColor(isEncased ? sf::Color::White : sf::Color(80, 130, 60));
        body.setOutlineColor(sf::Color(30, 180, 30));
        body.setOutlineThickness(4);
        window.draw(body);
    }

    if (!isEncased) {
        window.draw(healthBarBg);
        window.draw(healthBarFill);
    }
}

void Gamakichi::drawDebug(sf::RenderWindow& window) {
    Enemy::drawDebug(window);
    for (int i = 0; i < rocketCount; i++) {
        if (rockets[i] && rockets[i]->isActive()) rockets[i]->drawDebug(window);
    }
}