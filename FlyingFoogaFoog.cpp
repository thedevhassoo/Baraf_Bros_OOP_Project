#include "FlyingFoogaFoog.h"
#include "Player.h"
#include <cstdlib>

FlyingFoogaFoog::FlyingFoogaFoog() {
    setEnemyName("FlyingFoogaFoog");
    setMaxHits(2);
    setScoreValue(200 + (rand() % 601));    // 200–800
    setGemDrop(1 + (rand() % 3));           // 1–3 gems
    rotationAngle = 0;
    isFlying = false;
    groundTimer = 0;
    groundDuration = 3.0f + (rand() % 200) / 100.0f;
    flightTimer = 0;
    flightDuration = 4.5f + (rand() % 150) / 100.0f;
    flightSpeedX = 100.0f;
    flightSpeedY = 60.0f;
    flightDirX = 1;
    flightDirY = -1;
}

FlyingFoogaFoog::~FlyingFoogaFoog() {}

void FlyingFoogaFoog::enterFlight() {
    isFlying = true;
    gravity = 0;

    // Always move — never 0 speed
    flightDirX = (rand() % 2 == 0) ? 1 : -1;
    flightDirY = (rand() % 2 == 0) ? 1 : -1;

    // Ensure minimum speed
    flightSpeedX = (80.0f + (rand() % 80)) * speedMultiplier;
    flightSpeedY = (50.0f + (rand() % 50)) * speedMultiplier;
}

void FlyingFoogaFoog::exitFlight() {
    isFlying = false;
    gravity = 980.0f;
    flightTimer = 0;
}

void FlyingFoogaFoog::updateAI(float dt, Platform** platforms, int platformCount,
    Player* player1, Player* player2) {
    if (!isFlying) {
        groundTimer += dt;
        if (groundTimer >= groundDuration) {
            groundTimer = 0;
            enterFlight();
        }
        Botom::updateAI(dt, platforms, platformCount, player1, player2);
    }
    else {
        flightTimer += dt;
        if (flightTimer >= flightDuration) {
            exitFlight();
            return;
        }

        // Bounce at edges
        if (posX < -10) {
            flightDirX = 1;
            posX = -9;
        }
        if (posX > 1280 - 22) {
            flightDirX = -1;
            posX = 1280 - 23;
        }
        if (posY < -10) {
            flightDirY = 1;
            posY = -9;
        }
        if (posY > 720 - 22) {
            flightDirY = -1;
            posY = 720 - 23;
        }

        velocityX = flightDirX * flightSpeedX;
        velocityY = flightDirY * flightSpeedY;
        facingRight = (flightDirX > 0);

        
    }

    // Animation
    if (hasEnemySpritesheet) {
        if (isEncased) {
            enemyCurrentAnim = 2;   // Hit
        }
        else if (isFlying) {
			enemyCurrentAnim = 0;   // Fly
        }
        else {
            enemyCurrentAnim = 0;   // Fly/Move
        }
    }
}

void FlyingFoogaFoog::draw(sf::RenderWindow& window) {
    if (hasEnemySpritesheet && enemySpritesheet) {
        if (isEncased) {
            enemyCurrentAnim = 2;   // Hit
        }
        else if (isFlying) {
            enemyCurrentAnim = 0;   // Fly
        }
        else {
            enemyCurrentAnim = 0;   // Fly/Move
        }

        if (enemyCurrentAnim != enemyPrevAnim) {
            enemySpritesheet->playAnim(enemyCurrentAnim);
            enemyPrevAnim = enemyCurrentAnim;
        }
        
        enemySpritesheet->draw(window);
    }
    else {
        sf::RectangleShape rect(sf::Vector2f(32, 32));
        rect.setPosition(posX, posY);
        if (isEncased) rect.setFillColor(sf::Color::White);
        else if (isFlying) rect.setFillColor(sf::Color(180, 100, 220));
        else rect.setFillColor(sf::Color(200, 50, 150));
        window.draw(rect);
    }
}