#include "Spritesheet.h"
#include<iostream>

Spritesheet::Spritesheet(int n, std::string path, const sf::Vector2i size) {
    sheet.loadFromFile(path);
    frameSize = sf::IntRect(0, 0, size.x, size.y);
    textureSprite.setTexture(sheet);
    textureSprite.setTextureRect(frameSize);
    animations = n;
    animArray = new sf::IntRect * [n];
    frameCount = new int[n];
    frameDuration = new float[n];
    animLoop = new bool[n];
    currentAnim = 0;
    currentFrame = 0;
    animationTimer = 0.0f;

    for (int i = 0; i < n; i++) {
        animArray[i] = nullptr;
        frameCount[i] = 0;
        frameDuration[i] = 0.1f;
        animLoop[i] = true;
    }
    sheetSize = sheet.getSize();
}

Spritesheet::~Spritesheet() {
    for (int i = 0; i < animations; i++) {
        if (animArray[i]) delete[] animArray[i];
    }
    delete[] frameDuration;
    delete[] frameCount;
    delete[] animArray;
    delete[] animLoop;
}

void Spritesheet::createAnim(int anim, int frames, const sf::Vector2i start, const sf::Vector2i size, float animSpeed) {
    animArray[anim] = new sf::IntRect[frames];
    frameCount[anim] = frames;
    frameDuration[anim] = 1.0f / animSpeed;

    sf::Vector2i pos(start.x, start.y);

    for (int i = 0; i < frames; i++) {
        // Wrap to next row if this frame would exceed sheet width
        if (pos.x + size.x > (int)sheetSize.x) {
            pos.x = 0;
            pos.y += size.y;
        }

        animArray[anim][i] = sf::IntRect(pos.x, pos.y, size.x, size.y);
        pos.x += size.x;
    }
}

void Spritesheet::setAnimLoop(int anim, bool loop) {
    if (anim >= 0 && anim < animations) {
        animLoop[anim] = loop;
    }
}

bool Spritesheet::isAnimFinished() const {
    return !animLoop[currentAnim] && currentFrame == frameCount[currentAnim] - 1;
}

void Spritesheet::playAnim(int n) {
    if (n != currentAnim) {
        currentAnim = n;
        currentFrame = 0;
        animationTimer = 0.0f;
        textureSprite.setTextureRect(animArray[currentAnim][0]);
        //std::cout << "playAnim " << n << " frames=" << frameCount[n] << " rect="
            //<< animArray[n][0].left << "," << animArray[n][0].top << std::endl;
    }
}

void Spritesheet::update(float dt) {
    animationTimer += dt;

    if (animationTimer >= frameDuration[currentAnim]) {
        animationTimer -= frameDuration[currentAnim];

        if (animLoop[currentAnim]) {
            currentFrame = (currentFrame + 1) % frameCount[currentAnim];
        }
        else {
            if (currentFrame < frameCount[currentAnim] - 1) {
                currentFrame++;
            }
        }
        textureSprite.setTextureRect(animArray[currentAnim][currentFrame]);
        //std::cout << "Frame " << currentFrame << " of anim " << currentAnim << std::endl;
    }
}

void Spritesheet::draw(sf::RenderWindow& window) {
    window.draw(textureSprite);
}
void Spritesheet::setRotation(float angle) {
    textureSprite.setRotation(angle);
}
void Spritesheet::setSpriteOrigin(float x, float y) {
    textureSprite.setOrigin(x, y);
}

void Spritesheet::setSpritePosition(float x, float y) {
    textureSprite.setPosition(x, y);
}

void Spritesheet::setSpriteScale(float sx, float sy) {
    textureSprite.setScale(sx, sy);
}