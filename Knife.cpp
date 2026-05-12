#include "Knife.h"
#include<iostream>


sf::Texture Knife::sharedTexture;
bool Knife::textureLoaded = false;

void Knife::loadTexture() {
    if (!textureLoaded) {
        sharedTexture.loadFromFile("./assets/projectiles/knife.png");
        textureLoaded = true;
    }
}

Knife::Knife()
    : posX(0), posY(0), velocityX(0), velocityY(0), speed(300.0f),
    lifetime(0), maxLifetime(2.5f), active(false), angle(0), hasTexture(false) {

    hitbox = new HitBox(0, 0, 16, 8);
    loadTexture();
    sprite.setTexture(sharedTexture);
    sprite.setOrigin(sharedTexture.getSize().x / 2.0f, sharedTexture.getSize().y / 2.0f);
    hasTexture = true;
}

Knife::~Knife() {
    if (hitbox) delete hitbox;
}

void Knife::setTexture(const char* path) {
    if (texture.loadFromFile(path)) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        hasTexture = true;
    }
}

void Knife::launch(float x, float y, float dirX, float dirY, float spd) {
    posX = x;
    posY = y;
    speed = spd;
    velocityX = dirX * speed;
    velocityY = dirY * speed;
    lifetime = 0;
    active = true;
    angle = atan2(dirY, dirX) * 180.0f / 3.14159f;
    hitbox->setPosition(x - 8, y - 4);
}

void Knife::update(float dt) {
    if (!active) return;
    lifetime += dt;
    if (lifetime >= maxLifetime) { deactivate(); return; }

    posX += velocityX * dt;
    posY += velocityY * dt;
    hitbox->setPosition(posX - 8, posY - 4);

    if (posX < -30 || posX > 1310 || posY < -30 || posY > 750) {
        deactivate();
    }
}

void Knife::draw(sf::RenderWindow& window) {
    //std::cout << " pos=" << posX << "," << posY << std::endl;
    if (!active) return;

    if (hasTexture) {
        sprite.setPosition(posX, posY);
        sprite.setRotation(angle);
        window.draw(sprite);
    }
    else {
        // Fallback
        sf::RectangleShape visual(sf::Vector2f(16, 4));
        visual.setPosition(posX - 8, posY - 2);
        visual.setFillColor(sf::Color(200, 200, 200));
        visual.setRotation(angle);
        window.draw(visual);
    }
}

void Knife::deactivate() { active = false; }
bool Knife::isActive() const { return active; }
float Knife::getX() const { return posX; }
float Knife::getY() const { return posY; }
HitBox* Knife::getHitbox() { return hitbox; }

void Knife::drawDebug(sf::RenderWindow& window) {
    if (hitbox && active) {
        hitbox->drawDebug(window, sf::Color::Yellow);
    }
}