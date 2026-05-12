#pragma once
#include <SFML/Graphics.hpp>
#include "HitBox.h"

class Knife {
private:
    float posX;
    float posY;
    float velocityX;
    float velocityY;
    float speed;
    float lifetime;
    float maxLifetime;
    bool active;
    float angle;

    HitBox* hitbox;
    sf::Texture texture;
    sf::Sprite sprite;
    bool hasTexture;
    static sf::Texture sharedTexture;
    static bool textureLoaded;

public:
    static void loadTexture();
    Knife();
    ~Knife();

    void setTexture(const char* path);
    void launch(float x, float y, float dirX, float dirY, float spd);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void deactivate();

    bool isActive() const;
    float getX() const;
    float getY() const;
    HitBox* getHitbox();
    void drawDebug(sf::RenderWindow& window);
};