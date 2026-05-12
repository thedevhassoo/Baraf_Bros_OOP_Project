#pragma once
#include <SFML/Graphics.hpp>
#include "HitBox.h"

class Snowball {
private:
    float posX;
    float posY;
    float velocityX;
    float speed;
    float distanceTraveled;
    float maxDistance;
    bool active;
    int power;
    static sf::Texture sharedTexture;
    static bool textureLoaded;
    HitBox* hitbox;
    sf::Texture texture;
    sf::Sprite sprite;
    bool hasTexture;

public:
    static void loadTexture();
    Snowball();
    ~Snowball();
    void setTexture(const char* path);
    void launch(float x, float y, float dirX, int pwr, float spd);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void deactivate();

    bool isActive() const;
    float getX() const;
    float getY() const;
    int getPower() const;
    HitBox* getHitbox();
    void drawDebug(sf::RenderWindow& window);
};