#include "ShopSystem.h"
#include "AuthManager.h"
#include "Player.h"

ShopSystem::ShopSystem() : auth(nullptr), player(nullptr) {}
ShopSystem::~ShopSystem() {}

void ShopSystem::setAuthManager(AuthManager* a) { auth = a; }
void ShopSystem::setPlayer(Player* p) { player = p; }

bool ShopSystem::buyExtraLife() {
    if (!auth) return false;
    if (auth->getGems() < getExtraLifeCost()) return false;
    auth->removeGems(getExtraLifeCost());
    auth->addSecondChance(1);
    return true;
}

bool ShopSystem::buySpeedBoost() {
    if (!auth) return false;
    if (auth->getGems() < getSpeedBoostCost()) return false;
    auth->removeGems(getSpeedBoostCost());
    auth->addSpeedBoost(1);
    return true;
}

bool ShopSystem::buySnowballPower() {
    if (!auth) return false;
    if (auth->getGems() < getSnowballPowerCost()) return false;
    auth->removeGems(getSnowballPowerCost());
    auth->addBalloons(1);
    return true;
}

bool ShopSystem::buyDistanceIncrease() {
    if (!auth) return false;
    if (auth->getGems() < getDistanceIncreaseCost()) return false;
    auth->removeGems(getDistanceIncreaseCost());
    auth->addDistancePUP(1);
    return true;
}

bool ShopSystem::buyBalloonMode() {
    if (!auth) return false;
    if (auth->getGems() < getBalloonModeCost()) return false;
    auth->removeGems(getBalloonModeCost());
    return true;
}


int ShopSystem::getExtraLifeCost() const { return 500; }
int ShopSystem::getSpeedBoostCost() const { return 200; }
int ShopSystem::getSnowballPowerCost() const { return 300; }
int ShopSystem::getDistanceIncreaseCost() const { return 250; }
int ShopSystem::getBalloonModeCost() const { return 350; }