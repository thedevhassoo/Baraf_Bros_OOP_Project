#pragma once
#include <SFML/Graphics.hpp>

class HitBox {
private:
    float posX;
    float posY;
    float width;
    float height;
    bool active;

public:
    HitBox();
    HitBox(float x, float y, float w, float h);
    ~HitBox();

    // Getters
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    bool isActive() const;

    // Setters
    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setActive(bool a);

    // Check if this hitbox overlaps another
    bool intersects(const HitBox& other) const;

    // Check if a point is inside this hitbox
    bool containsPoint(float px, float py) const;

    // Draw debug outline
    void drawDebug(sf::RenderWindow& window, sf::Color color) const;
};