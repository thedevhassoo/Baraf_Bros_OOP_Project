#include "HitBox.h"

HitBox::HitBox() : posX(0), posY(0), width(0), height(0), active(true) {}

HitBox::HitBox(float x, float y, float w, float h)
    : posX(x), posY(y), width(w), height(h), active(true) {
}

HitBox::~HitBox() {}

float HitBox::getX() const { return posX; }
float HitBox::getY() const { return posY; }
float HitBox::getWidth() const { return width; }
float HitBox::getHeight() const { return height; }
float HitBox::getLeft() const { return posX; }
float HitBox::getRight() const { return posX + width; }
float HitBox::getTop() const { return posY; }
float HitBox::getBottom() const { return posY + height; }
bool HitBox::isActive() const { return active; }

void HitBox::setPosition(float x, float y) { posX = x; posY = y; }
void HitBox::setSize(float w, float h) { width = w; height = h; }
void HitBox::setActive(bool a) { active = a; }

bool HitBox::intersects(const HitBox& other) const {
    if (!active || !other.active) return false;
    if (getRight() <= other.getLeft()) return false;
    if (getLeft() >= other.getRight()) return false;
    if (getBottom() <= other.getTop()) return false;
    if (getTop() >= other.getBottom()) return false;
    return true;
}

bool HitBox::containsPoint(float px, float py) const {
    if (!active) return false;
    return (px >= getLeft() && px <= getRight() && py >= getTop() && py <= getBottom());
}

void HitBox::drawDebug(sf::RenderWindow& window, sf::Color color) const {
    if (!active) return;
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(posX, posY);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(color);
    rect.setOutlineThickness(2.0f);
    window.draw(rect);
}