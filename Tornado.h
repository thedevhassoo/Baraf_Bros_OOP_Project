#pragma once
#include "FlyingFoogaFoog.h"
#include <SFML/Audio.hpp>
class Knife;

class Tornado : public FlyingFoogaFoog {
private:
    bool canThrowKnife;
    float knifeTimer;
    float knifeCooldown;
    float teleportTimer;
    float teleportInterval;

    void throwKnife(Player* player);
    void doTeleport();

    sf::SoundBuffer knifeSoundBuffer;
    sf::Sound knifeSound;
    bool hasKnifeSound;

public:
    Knife** knives;
    int knifeCount;
    int knifeCapacity;

    Tornado();
    ~Tornado();

    void updateAI(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2) override;
    void draw(sf::RenderWindow& window) override;
    void drawDebug(sf::RenderWindow& window) override;

    void addKnife(Knife* k);
    void removeKnife(int index);
    void setKnifeSound(const char* path);
};