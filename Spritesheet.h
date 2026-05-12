#pragma once
#include <SFML/Graphics.hpp>

class Spritesheet {
private:
    sf::Texture sheet;
    sf::IntRect** animArray;
    sf::Vector2u sheetSize;
    sf::IntRect frameSize;
    int* frameCount;
    float* frameDuration;
    bool* animLoop;

    int animations;
    int currentAnim;
    int currentFrame;
    float animationTimer;

    sf::Sprite textureSprite;

public:
    Spritesheet(int n, std::string path, sf::Vector2i size);
    ~Spritesheet();

    void createAnim(int anim, int frames, sf::Vector2i start, sf::Vector2i size, float animSpeed);
    void setAnimLoop(int anim, bool loop);
    bool isAnimFinished() const;
    void playAnim(int n);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    void setSpriteOrigin(float x, float y);
    void setSpritePosition(float x, float y);
    void setSpriteScale(float sx, float sy);
    void setRotation(float angle);
};