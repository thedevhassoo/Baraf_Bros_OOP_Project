#pragma once
#include <SFML/Graphics.hpp>
#include "HitBox.h"

class Platform;

class ArtilleryRocket {
private:
    float posX, posY;
    float velocityX, velocityY;
    float speed;
    bool active, exploded;
    float explosionTimer, explosionDuration;
    float explosionRadius;

    HitBox* hitbox;

    static sf::Texture bombTexture;
    static sf::Texture explosionTexture;
    static bool texturesLoaded;
    sf::Sprite sprite;
    float targetPosX;
    float targetPosY;
public:
    ArtilleryRocket();
    ~ArtilleryRocket();

    static void loadTextures();
    void launch(float x, float y, float targetX, float targetY, float spd);
    void explode();
    void update(float dt, Platform** platforms, int platformCount);
    void draw(sf::RenderWindow& window);

    bool isActive() const;
    bool hasExploded() const;
    float getX() const;
    float getY() const;
    float getExplosionRadius() const;
    HitBox* getHitbox();
    void drawDebug(sf::RenderWindow& window);
};