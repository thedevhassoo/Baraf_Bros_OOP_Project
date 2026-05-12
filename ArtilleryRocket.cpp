#include "ArtilleryRocket.h"
#include "Platform.h"
#include "Tile.h"
#include <cmath>

sf::Texture ArtilleryRocket::bombTexture;
sf::Texture ArtilleryRocket::explosionTexture;
bool ArtilleryRocket::texturesLoaded = false;

void ArtilleryRocket::loadTextures() {
    if (!texturesLoaded) {
        bombTexture.loadFromFile("./assets/projectiles/bomb.png");
        explosionTexture.loadFromFile("./assets/projectiles/explosion.png");
        texturesLoaded = true;
    }
}

ArtilleryRocket::ArtilleryRocket()
    : posX(0), posY(0), velocityX(0), velocityY(0), speed(200.0f),
    active(false), exploded(false),
    explosionTimer(0), explosionDuration(0.5f), explosionRadius(50.0f) {

    loadTextures();
    sprite.setTexture(bombTexture);
    sprite.setOrigin(bombTexture.getSize().x / 2.0f, bombTexture.getSize().y / 2.0f);
    hitbox = new HitBox(0, 0, 14, 14);
}

ArtilleryRocket::~ArtilleryRocket() {
    if (hitbox) delete hitbox;
}
void ArtilleryRocket::launch(float x, float y, float targetX, float targetY, float spd) {
    posX = x;
    posY = y;
    speed = spd;

    // Time to hit ground from this height with gravity
    // Use simple estimate: total flight time ~ 1.5 seconds at speed 400
    float flightTime = 1.5f;

    // X velocity to reach target in flight time
    float dx = targetX - x;
    velocityX = dx / flightTime;

    // Y velocity to arc up then down
    velocityY = -speed * 0.6f;

    active = true;
    exploded = false;
    explosionTimer = 0;

    sprite.setTexture(bombTexture);
    float hw = bombTexture.getSize().x * 0.5f;
    float hh = bombTexture.getSize().y * 0.5f;
    sprite.setOrigin(hw, hh);
    hitbox->setSize(hw, hh);
    hitbox->setPosition(x - hw / 2.0f, y - hh / 2.0f);
}

void ArtilleryRocket::update(float dt, Platform** platforms, int platformCount) {
    if (!active) return;

    if (exploded) {
        explosionTimer += dt;
        if (explosionTimer >= explosionDuration) active = false;
        return;
    }

    velocityY += 600.0f * dt;
    posX += velocityX * dt;
    posY += velocityY * dt;
    hitbox->setPosition(posX - 14, posY - 14);

    // Explode when hitting ground
    if (posY >= 700-64) {
        posY = 700-64;
        explode();
    }

    // Off screen
    if (posX < -50 || posX > 1330) active = false;
}


void ArtilleryRocket::explode() {
    exploded = true;
    velocityX = 0;
    velocityY = 0;
    explosionTimer = 0;

    sprite.setTexture(explosionTexture);
    // Use the FULL texture — don't set a texture rect
    sprite.setTextureRect(sf::IntRect(0, 0, explosionTexture.getSize().x, explosionTexture.getSize().y));
    sprite.setOrigin(explosionTexture.getSize().x / 2.0f, explosionTexture.getSize().y / 2.0f);
    sprite.setRotation(0);
    sprite.setScale(1.0f, 1.0f);

    hitbox->setSize(explosionRadius * 2, explosionRadius * 2);
    hitbox->setPosition(posX - explosionRadius, posY - explosionRadius);
}



void ArtilleryRocket::draw(sf::RenderWindow& window) {
    if (!active) return;
    sprite.setPosition(posX, posY);
    window.draw(sprite);
}

bool ArtilleryRocket::isActive() const { return active; }
bool ArtilleryRocket::hasExploded() const { return exploded; }
float ArtilleryRocket::getX() const { return posX; }
float ArtilleryRocket::getY() const { return posY; }
float ArtilleryRocket::getExplosionRadius() const { return explosionRadius; }
HitBox* ArtilleryRocket::getHitbox() { return hitbox; }

void ArtilleryRocket::drawDebug(sf::RenderWindow& window) {
    if (!active) return;
    if (exploded) {
        sf::CircleShape blast(explosionRadius);
        blast.setPosition(posX - explosionRadius, posY - explosionRadius);
        blast.setFillColor(sf::Color::Transparent);
        blast.setOutlineColor(sf::Color(255, 150, 0, 150));
        blast.setOutlineThickness(2);
        window.draw(blast);
    }
    else if (hitbox) {
        hitbox->drawDebug(window, sf::Color::Yellow);
    }
}