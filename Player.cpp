#include "Player.h"
#include "Platform.h"
#include "Tile.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
    : playerIndex(1), lives(2), snowballPower(1), snowballDistance(300.0f),
    moveSpeed(200.0f), jumpForce(-700.0f),
    keyLeft(0), keyRight(0), keyJump(0), keyThrow(0),
    throwCooldown(0.15f), throwTimer(0), canThrowSnowball(true),
    speedBoosted(false), speedBoostTimer(0),
    snowballPowered(false), distanceIncreased(false), balloonMode(false), balloonTimer(0),
    spritesheet(nullptr), currentAnim(0), animState(ANIM_IDLE),
    hasJumpSound(false), hasThrowSound(false) {
    isDying = false;
    dyingTimer = 0;
    hitTimer = 0;
    gravity = 980.0f;
    setHitboxSize(40, 78);
    setHitboxOffset(0, -15);
}

Player::~Player() {
    if (spritesheet) delete spritesheet;
}

void Player::init(int playerIdx, float startX, float startY) {
    playerIndex = playerIdx;
    posX = startX;
    posY = startY;

    updateHitboxPosition();
}

void Player::setKeys(int left, int right, int jump, int throwKey) {
    keyLeft = left;
    keyRight = right;
    keyJump = jump;
    keyThrow = throwKey;
}

void Player::setSpritesheet(Spritesheet* sheet) {
    spritesheet = sheet;
    //spritesheet->setSpriteOrigin(69, 58);   // Half of frame size
}

void Player::setJumpSound(const char* path) {
    if (jumpSoundBuffer.loadFromFile(path)) {
        jumpSound.setBuffer(jumpSoundBuffer);
        hasJumpSound = true;
    }
}

void Player::setThrowSound(const char* path) {
    if (throwSoundBuffer.loadFromFile(path)) {
        throwSound.setBuffer(throwSoundBuffer);
        hasThrowSound = true;
    }
}

int Player::getPlayerIndex() const { return playerIndex; }
int Player::getLives() const { return lives; }
float Player::getMoveSpeed() const { return speedBoosted ? moveSpeed * 1.5f : moveSpeed; }
bool Player::isSpeedBoosted() const { return speedBoosted; }
bool Player::isBalloonMode() const { return balloonMode; }
bool Player::canThrow() const { return canThrowSnowball && throwTimer <= 0; }
void Player::setLives(int l) { lives = l; }

void Player::moveLeft() {
    velocityX = -getMoveSpeed();
    facingRight = false;
}

void Player::moveRight() {
    velocityX = getMoveSpeed();
    facingRight = true;
}

void Player::stopHorizontal() {
    velocityX = 0;
}

void Player::jump() {
    velocityY = jumpForce;
    isGrounded = false;
    if (hasJumpSound) jumpSound.play();
}

void Player::throwSnowball() {
    canThrowSnowball = false;
    throwTimer = throwCooldown;
    if (hasThrowSound) throwSound.play();
}

void Player::resetThrowCooldown() {
    canThrowSnowball = false;
    throwTimer = throwCooldown;
}
void Player::takeDamage() {
    if (isDying || hitTimer > 0) return;
    hitTimer = 2.0f;
    lives--;

    if (lives <= 0) {
        // Play death animation
        isDying = true;
        dyingTimer = 1.0f;
    }

    velocityX = 0;
    velocityY = 0;
}
void Player::addLife() { lives++; }

void Player::applySpeedBoost(float duration) {
    speedBoosted = true;
    speedBoostTimer = duration;
}

void Player::applySnowballPower() {
    snowballPowered = true;
    snowballPower = 2;
}

void Player::applyDistanceIncrease() {
    distanceIncreased = true;
    snowballDistance = 600.0f;
}

void Player::applyBalloonMode(float duration) {
    balloonMode = true;
    balloonTimer = duration;
    gravity = 0;
}

void Player::updatePowerUps(float dt) {
    if (speedBoosted) {
        speedBoostTimer -= dt;
        if (speedBoostTimer <= 0) speedBoosted = false;
    }
    if (balloonMode) {
        balloonTimer -= dt;
        if (balloonTimer <= 0) {
            balloonMode = false;
            gravity = 980.0f;
        }
    }
}

void Player::clearPowerUps() {
    speedBoosted = false;
    snowballPowered = false;
    snowballPower = 1;
    distanceIncreased = false;
    snowballDistance = 300.0f;
    balloonMode = false;
    gravity = 980.0f;
}

void Player::checkPlatformCollisions(Platform** platforms, int count) {
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

//void Player::updateAnimation(float dt) {
//    if (balloonMode || !isGrounded) {
//        animState = ANIM_JUMP;
//        currentAnim = 2;
//    }
//    else if (velocityX != 0) {
//        animState = ANIM_WALK;
//        currentAnim = 1;
//    }
//    else {
//        animState = ANIM_IDLE;
//        currentAnim = 0;
//    }
//}
void Player::updateAnimation(float dt) {
    if (isDying) {
        currentAnim = 5;   // Death
    }
     else if (!canThrowSnowball && throwTimer > throwCooldown - 0.15f) {
        currentAnim = 3;
    }
    else if (balloonMode || !isGrounded) {
        animState = ANIM_JUMP;
        currentAnim = 2;
    }
    else if (velocityX != 0) {
        animState = ANIM_WALK;
        currentAnim = 1;
    }
    else {
        animState = ANIM_IDLE;
        currentAnim = 0;
    }

    //if (currentAnim != prevAnim) {
    //    if (spritesheet) spritesheet->playAnim(currentAnim);
    //    prevAnim = currentAnim;
    //}
}
void Player::update(float dt) {
    if (isDying) {
        dyingTimer -= dt;
        if (spritesheet) { spritesheet->update(dt); updateAnimation(dt); }
        applyPhysics(dt);
        if (dyingTimer <= 0) {
            isDying = false;
        }
        return;
    }

    // Invincibility timer
    if (hitTimer > 0) {
        hitTimer -= dt;
    }
    bool leftPressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyLeft);
    bool rightPressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyRight);
    bool jumpPressed = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyJump);

    if (balloonMode) {
        velocityX = 0;
        velocityY = -80.0f;
    }
    else {
        if (leftPressed) moveLeft();
        else if (rightPressed) moveRight();
        else stopHorizontal();

        if (jumpPressed && isGrounded) jump();
    }

    if (!canThrowSnowball) {
        throwTimer -= dt;
        if (throwTimer <= 0) {
            canThrowSnowball = true;
            throwTimer = 0;
        }
    }

    updatePowerUps(dt);
    if (!balloonMode) gravity = 980.0f;

    applyPhysics(dt);
    updateAnimation(dt);

    if (posX < 0) posX = 0;
    if (posX > 1280 - hitbox->getWidth()) posX = 1280 - hitbox->getWidth();
    if (posY < 0) { posY = 0; velocityY = 0; }
    if (posY > 720 - hitbox->getHeight()) posY = 720 - hitbox->getHeight();
    updateHitboxPosition();
    spritesheet->setSpritePosition(posX + 16, posY + 24);

    if (!facingRight) {
        spritesheet->setSpriteScale(-1.0f, 1.0f);
    }
    else {
        spritesheet->setSpriteScale(1.0f, 1.0f);
    }
    if (spritesheet) {
        spritesheet->update(dt);
    }
}

void Player::draw(sf::RenderWindow& window) {
    if (spritesheet) {
        //spritesheet->setSpritePosition(posX, posY);
        //if (!facingRight) {
        //    spritesheet->setSpriteScale(-1.0f, 1.0f);
        //    spritesheet->setSpritePosition(posX + 32, posY);
        //} else {
        //    spritesheet->setSpriteScale(1.0f, 1.0f);
        //}
       
            spritesheet->playAnim(currentAnim);
        
        spritesheet->draw(window);
    } else {
        sf::RectangleShape rect(sf::Vector2f(32, 48));
        rect.setPosition(posX, posY);
        rect.setFillColor(balloonMode ? sf::Color::Cyan : sf::Color(50, 100, 255));
        window.draw(rect);
    }
}

void Player::drawDebug(sf::RenderWindow& window) {
    if (hitbox) {
        hitbox->drawDebug(window, sf::Color::Green);
    }
}