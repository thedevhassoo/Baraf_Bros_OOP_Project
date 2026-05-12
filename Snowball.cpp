#include "Snowball.h"
sf::Texture Snowball::sharedTexture;
bool Snowball::textureLoaded = false;

void Snowball::loadTexture() {
    if (!textureLoaded) {
        sharedTexture.loadFromFile("./assets/projectiles/snowball.png");
        textureLoaded = true;
    }
}
Snowball::Snowball()
    : posX(0), posY(0), velocityX(0), speed(600.0f), distanceTraveled(0),
    maxDistance(700.0f), active(false), power(1), hasTexture(false) {

    hitbox = new HitBox(0, 0, 16, 16);
    loadTexture();
    sprite.setTexture(sharedTexture);
    sprite.setOrigin(sharedTexture.getSize().x / 2.0f, sharedTexture.getSize().y / 2.0f);
    hasTexture = true;
}

Snowball::~Snowball() {
    if (hitbox) delete hitbox;
}

void Snowball::setTexture(const char* path) {
    if (texture.loadFromFile(path)) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        hasTexture = true;
    }
}

void Snowball::launch(float x, float y, float dirX, int pwr, float spd) {
    posX = x;
    posY = y;
    velocityX = dirX * spd;
    speed = spd;
    power = pwr;
    distanceTraveled = 0;
    active = true;
    hitbox->setPosition(x - 8, y - 8);
}

void Snowball::update(float dt) {
    if (!active) return;

    float moveX = velocityX * dt;
    posX += moveX;
    distanceTraveled += (moveX > 0) ? moveX : -moveX;

    //if (posX > 1294) posX = -16;
    //if (posX < -16) posX = 1294;

    hitbox->setPosition(posX - 8, posY - 8);
    if (posX < -20 || posX > 1300) {
        deactivate();
    }
    if (distanceTraveled >= maxDistance) {
        deactivate();
    }
}

void Snowball::draw(sf::RenderWindow& window) {
    if (!active) return;

    if (hasTexture) {
        sprite.setPosition(posX, posY);
        // Rotate based on direction
        sprite.setScale(velocityX > 0 ? 1.0f : -1.0f, 1.0f);
        window.draw(sprite);
    }
    else {
        sf::CircleShape visual(8);
        visual.setPosition(posX - 8, posY - 8);
        visual.setFillColor(sf::Color(200, 220, 255));
        visual.setOutlineColor(sf::Color::White);
        visual.setOutlineThickness(1);
        window.draw(visual);
    }
}

void Snowball::deactivate() { active = false; }
bool Snowball::isActive() const { return active; }
float Snowball::getX() const { return posX; }
float Snowball::getY() const { return posY; }
int Snowball::getPower() const { return power; }
HitBox* Snowball::getHitbox() { return hitbox; }

void Snowball::drawDebug(sf::RenderWindow& window) {
    if (hitbox && active) {
        hitbox->drawDebug(window, sf::Color::Yellow);
    }
}