#include "Botom.h"
#include "Player.h"
#include "Platform.h"
#include "Tile.h"
#include <cstdlib>

Botom::Botom() {
    setEnemyName("Botom");
    setMaxHits(1);
    setScoreValue(100 + (rand() % 401));    // 100–500
    setGemDrop(rand() % 3);                 // 0–2 gems
    setRollSpeed(350.0f);
    setEncaseDuration(4.0f);

    aiTimer = 0;
    aiInterval = 1.5f + (rand() % 100) / 100.0f;
    moveDirection = 0;
    walkSpeed = 80.0f;

    setHitboxSize(28, 32);
    setHitboxOffset(2, 0);
    gravity = 980.0f;
}

Botom::~Botom() {}

void Botom::pickNewDirection() {
    int r = rand() % 3;
    if (r == 0) moveDirection = -1;
    else if (r == 1) moveDirection = 1;
    else moveDirection = 0;
}

void Botom::updateAI(float dt, Platform** platforms, int platformCount,
    Player* player1, Player* player2) {
    aiTimer += dt;
    if (aiTimer >= aiInterval) {
        aiTimer = 0;
        pickNewDirection();
        aiInterval = 1.2f + (rand() % 150) / 100.0f;
    }

    velocityX = moveDirection * walkSpeed * speedMultiplier;
    facingRight = (moveDirection > 0);

    if (isGrounded && moveDirection != 0) {
        float checkX = posX + (moveDirection > 0 ? hitbox->getWidth() + 2 : -4);
        float checkY = posY + hitbox->getHeight() + 4;
        bool onPlatform = false;

        for (int i = 0; i < platformCount; i++) {
            if (!platforms[i]) continue;
            Tile* tile = platforms[i]->getHeadTile();
            while (tile) {
                if (checkX >= tile->getX() && checkX <= tile->getX() + tile->getWidth() &&
                    checkY >= tile->getY() && checkY <= tile->getY() + tile->getHeight()) {
                    onPlatform = true;
                    break;
                }
                tile = tile->getNext();
            }
            if (onPlatform) break;
        }

        // Floor check
        if (checkY >= 720) onPlatform = true;

        if (!onPlatform) {
            // At screen edge — always turn
            if ((moveDirection < 0 && posX < 10) || (moveDirection > 0 && posX > 1240)) {
                moveDirection *= -1;
                facingRight = !facingRight;
            }
            // Check if it's the floor — always turn on floor
            else if (checkY >= 700) {
                moveDirection *= -1;
                facingRight = !facingRight;
            }
            else if (rand() % 2 == 0) {
				
                moveDirection *= -1;
                facingRight = !facingRight;
            }
            else {
                //fallingOff = true;
                posX+= 100 * moveDirection;
            }
            // else: walk off edge
        }
        if (posX < 0) {
            posX = 0;
            moveDirection = 1;
            facingRight = true;
        }
        if (posX > 1280 - hitbox->getWidth()) {
            posX = 1280 - hitbox->getWidth();
            moveDirection = -1;
            facingRight = false;
        }
    }
    
}

void Botom::draw(sf::RenderWindow& window) {
    if (hasEnemySpritesheet && enemySpritesheet) {
       
        if (isEncased) enemyCurrentAnim = 2;
        else if (velocityX != 0) enemyCurrentAnim = 1;
        else enemyCurrentAnim = 0;

        if (enemyCurrentAnim != enemyPrevAnim) {
            enemySpritesheet->playAnim(enemyCurrentAnim);
            enemyPrevAnim = enemyCurrentAnim;
        }
        enemySpritesheet->draw(window);
    }
    else {
        sf::RectangleShape rect(sf::Vector2f(32, 32));
        rect.setPosition(posX, posY);
        if (isEncased) {
            rect.setFillColor(sf::Color::White);
        }
        else {
            switch (colorVariant) {
            case 0: rect.setFillColor(sf::Color(200, 50, 50)); break;
            case 1: rect.setFillColor(sf::Color(50, 200, 50)); break;
            case 2: rect.setFillColor(sf::Color(50, 50, 220)); break;
            }
        }
        window.draw(rect);
    }
}