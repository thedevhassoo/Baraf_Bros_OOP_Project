#include "Character2D.h"

Character2D::Character2D()
    : posX(0), posY(0), velocityX(0), velocityY(0),
    gravity(980.0f), isGrounded(false), facingRight(true),
    hitbox(nullptr), hitboxOffsetX(0), hitboxOffsetY(0) {
}

Character2D::~Character2D() {
    if (hitbox) {
        delete hitbox;
        hitbox = nullptr;
    }
}

void Character2D::applyPhysics(float dt) {
    if (!isGrounded) {
        velocityY += gravity * dt;
    }
    posX += velocityX * dt;
    posY += velocityY * dt;
    updateHitboxPosition();
}

void Character2D::updateHitboxPosition() {
    if (hitbox) {
        hitbox->setPosition(posX + hitboxOffsetX, posY + hitboxOffsetY);
    }
}

void Character2D::setPosition(float x, float y) { posX = x; posY = y; updateHitboxPosition(); }
void Character2D::setVelocity(float vx, float vy) { velocityX = vx; velocityY = vy; }
void Character2D::addVelocity(float vx, float vy) { velocityX += vx; velocityY += vy; }

float Character2D::getX() const { return posX; }
float Character2D::getY() const { return posY; }
float Character2D::getVelocityX() const { return velocityX; }
float Character2D::getVelocityY() const { return velocityY; }
float Character2D::getGravity() const { return gravity; }
bool Character2D::getIsGrounded() const { return isGrounded; }
bool Character2D::getFacingRight() const { return facingRight; }
HitBox* Character2D::getHitbox() { return hitbox; }

void Character2D::setGravity(float g) { gravity = g; }
void Character2D::setGrounded(bool g) { isGrounded = g; }
void Character2D::setFacingRight(bool f) { facingRight = f; }

void Character2D::setHitboxSize(float w, float h) {
    if (!hitbox) hitbox = new HitBox(0, 0, w, h);
    else hitbox->setSize(w, h);
}

void Character2D::setHitboxOffset(float ox, float oy) {
    hitboxOffsetX = ox;
    hitboxOffsetY = oy;
}

void Character2D::drawDebug(sf::RenderWindow& window) {
    if (hitbox) {
        hitbox->drawDebug(window, sf::Color::Green);
    }
}