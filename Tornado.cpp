#include "Tornado.h"
#include "Knife.h"
#include "Player.h"
#include <cstdlib>

Tornado::Tornado() {
    setEnemyName("Tornado");
    setMaxHits(2);
    setScoreValue(300 + (rand() % 901));    // 300–1200
    setGemDrop(2 + (rand() % 3));           // 2–4 gems

    canThrowKnife = true;
    knifeTimer = (rand() % 200) / 100.0f;  // Random 0-2 second offset
    knifeCooldown = 2.0f + (rand() % 200) / 100.0f;  // Random 2-4 seconds
    teleportTimer = 0;
    teleportInterval = 4.0f;
    hasKnifeSound = false;
    knifeCapacity = 6;
    knifeCount = 0;
    knives = new Knife * [knifeCapacity];
    for (int i = 0; i < knifeCapacity; i++) knives[i] = nullptr;
}

Tornado::~Tornado() {
    for (int i = 0; i < knifeCount; i++) {
        if (knives[i]) delete knives[i];
    }
    delete[] knives;
}
void Tornado::setKnifeSound(const char* path) {
    if (knifeSoundBuffer.loadFromFile(path)) {
        knifeSound.setBuffer(knifeSoundBuffer);
        hasKnifeSound = true;
    }
}
void Tornado::throwKnife(Player* target) {
    if (knifeCount >= knifeCapacity || !target) return;
    if (knifeCount >= knifeCapacity || !target) return;

    if (hasKnifeSound) {
        knifeSound.play();
    }

    Knife* k = new Knife();
    float dx = target->getX() - posX;
    float dy = target->getY() - posY;

    // Normalize
    float len = sqrt(dx * dx + dy * dy);
    if (len > 0.001f) {
        dx /= len;
        dy /= len;
    }
    else {
        dx = 1.0f;
        dy = 0.0f;
    }

    k->launch(posX + 16, posY + 16, dx, dy, 300.0f);
    addKnife(k);
    knifeTimer = 0;
}

void Tornado::doTeleport() {
    // Pick a random target position to fly toward
    float targetX = 100.0f + (rand() % 1000);   // 100-1100
    float targetY = 80.0f + (rand() % 300);     // 80-380

    // Set flight direction toward target
    float dx = targetX - posX;
    float dy = targetY - posY;

    if (dx > 0) flightDirX = 1;
    else flightDirX = -1;

    if (dy > 0) flightDirY = 1;
    else flightDirY = -1;

    // Set flight speed and enter flight mode
    flightSpeedX = 100.0f;
    flightSpeedY = 100.0f;

    if (!isFlying) {
        enterFlight();
    }

    teleportTimer = 0;
    teleportInterval = 5.0f + (rand() % 300) / 100.0f;
}

void Tornado::addKnife(Knife* k) {
    if (knifeCount >= knifeCapacity) {
        delete k;
        return;
    }
    knives[knifeCount++] = k;
}

void Tornado::removeKnife(int index) {
    if (index < 0 || index >= knifeCount) return;
    delete knives[index];
    for (int i = index; i < knifeCount - 1; i++) {
        knives[i] = knives[i + 1];
    }
    knives[knifeCount - 1] = nullptr;
    knifeCount--;
}

void Tornado::updateAI(float dt, Platform** platforms, int platformCount,
    Player* player1, Player* player2) {
    FlyingFoogaFoog::updateAI(dt, platforms, platformCount, player1, player2);

    // Teleport
    teleportTimer += dt;
    if (teleportTimer >= teleportInterval) {
        doTeleport();
    }

    // Knife throwing always try to throw, target closest player
    knifeTimer += dt;
    if (knifeTimer >= knifeCooldown) {
        // Find a valid target
        Player* target = nullptr;
        if (player1 && player2) {
            float d1 = (posX - player1->getX()) * (posX - player1->getX()) +
                (posY - player1->getY()) * (posY - player1->getY());
            float d2 = (posX - player2->getX()) * (posX - player2->getX()) +
                (posY - player2->getY()) * (posY - player2->getY());
            target = (d1 < d2) ? player1 : player2;
        }
        else if (player1) {
            target = player1;
        }
        else if (player2) {
            target = player2;
        }

        if (target) {
            throwKnife(target);
        }
    }

    // Update knives
    for (int i = 0; i < knifeCount; i++) {
        if (knives[i]) {
            knives[i]->update(dt);
            if (!knives[i]->isActive()) {
                removeKnife(i);
                i--;
            }
        }
    }
    
}

void Tornado::draw(sf::RenderWindow& window) {
    // Draw knives
    for (int i = 0; i < knifeCount; i++) {
        if (knives[i] && knives[i]->isActive()) {
            knives[i]->draw(window);
        }
    }

    // Draw body
    if (hasEnemySpritesheet && enemySpritesheet) {
        if (isEncased) {
            enemyCurrentAnim = 2;   // Hurt
        }
        else if (knifeTimer < 0.3f) {
            enemyCurrentAnim = 1;   // Attack (just threw knife)
        }
        else if (velocityX != 0 || isFlying) {
            enemyCurrentAnim = 0;   // Walk/Hover
        }
        else {
            enemyCurrentAnim = 0;   // Idle
        }

        if (enemyCurrentAnim != enemyPrevAnim) {
            enemySpritesheet->playAnim(enemyCurrentAnim);
            enemyPrevAnim = enemyCurrentAnim;
        }
        enemySpritesheet->draw(window);
    }
    else {
        sf::RectangleShape rect(sf::Vector2f(34, 34));
        rect.setPosition(posX, posY);
        if (isEncased) rect.setFillColor(sf::Color::White);
        else rect.setFillColor(sf::Color(160, 30, 30));
        window.draw(rect);
    }
}

void Tornado::drawDebug(sf::RenderWindow& window) {
    Enemy::drawDebug(window);
    for (int i = 0; i < knifeCount; i++) {
        if (knives[i] && knives[i]->isActive()) {
            knives[i]->drawDebug(window);
        }
    }
}