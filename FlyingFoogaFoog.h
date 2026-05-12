#pragma once
#include "Botom.h"

class FlyingFoogaFoog : public Botom {
protected:
    bool isFlying;
    float groundTimer;
    float groundDuration;
    float flightTimer;
    float flightDuration;
    float flightSpeedX;
    float flightSpeedY;
    int rotationAngle;
    int flightDirX;
    int flightDirY;

    void enterFlight();
    void exitFlight();

public:
    FlyingFoogaFoog();
    ~FlyingFoogaFoog();

    void updateAI(float dt, Platform** platforms, int platformCount,
        Player* player1, Player* player2) override;
    void draw(sf::RenderWindow& window) override;
};