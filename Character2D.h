#pragma once
#include <SFML/Graphics.hpp>
#include "HitBox.h"

class Character2D {
protected:
    float posX;
    float posY;
    float velocityX;
    float velocityY;
    float gravity;
    bool isGrounded;
    bool facingRight;
    HitBox* hitbox;
    float hitboxOffsetX;
    float hitboxOffsetY;

public:
    Character2D();
    virtual ~Character2D();

    // Simple physics update
    virtual void applyPhysics(float dt);
    virtual void updateHitboxPosition();

    // Position and velocity
    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    void addVelocity(float vx, float vy);

    // Getters
    float getX() const;
    float getY() const;
    float getVelocityX() const;
    float getVelocityY() const;
    float getGravity() const;
    bool getIsGrounded() const;
    bool getFacingRight() const;
    HitBox* getHitbox();

    // Setters
    void setGravity(float g);
    void setGrounded(bool g);
    void setFacingRight(bool f);
    void setHitboxSize(float w, float h);
    void setHitboxOffset(float ox, float oy);

    // Virtual update and draw
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void drawDebug(sf::RenderWindow& window);
};