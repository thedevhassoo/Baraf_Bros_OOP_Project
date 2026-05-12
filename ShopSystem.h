#pragma once

class AuthManager;
class Player;

class ShopSystem {
private:
    AuthManager* auth;
    Player* player;

public:
    ShopSystem();
    ~ShopSystem();

    void setAuthManager(AuthManager* a);
    void setPlayer(Player* p);

    // Shop items (spec 8.4)
    bool buyExtraLife();                    // 50 gems
    bool buySpeedBoost();                   // 20 gems
    bool buySnowballPower();                // 30 gems
    bool buyDistanceIncrease();             // 25 gems
    bool buyBalloonMode();                  // 35 gems

    int getExtraLifeCost() const;
    int getSpeedBoostCost() const;
    int getSnowballPowerCost() const;
    int getDistanceIncreaseCost() const;
    int getBalloonModeCost() const;
};