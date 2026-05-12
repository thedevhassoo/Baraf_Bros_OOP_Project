#include "EnemyFactory.h"
#include "Botom.h"
#include "FlyingFoogaFoog.h"
#include "Tornado.h"
#include "Mogera.h"
#include "Gamakichi.h"
#include "ColorVariant.h"
#include <cstring>

const char* EnemyFactory::typeNames[] = {
    "Botom",
    "FlyingFoogaFoog",
    "Tornado",
    "Mogera",
    "Gamakichi"
};
const int EnemyFactory::typeCount = 5;

Enemy* EnemyFactory::createEnemy(const char* type) {
    if (strcmp(type, "Botom") == 0) {
        return new Botom();
    }
    else if (strcmp(type, "FlyingFoogaFoog") == 0) {
        return new FlyingFoogaFoog();
    }
    else if (strcmp(type, "Tornado") == 0) {
        return new Tornado();
    }
    else if (strcmp(type, "Mogera") == 0) {
        return new Mogera();
    }
    else if (strcmp(type, "Gamakichi") == 0) {
        return new Gamakichi();
    }
    return nullptr;
}

Enemy* EnemyFactory::createEnemyWithVariant(const char* type, int variant) {
    Enemy* enemy = createEnemy(type);
    if (enemy && variant > 0) {
        ColorVariant::applyVariant(enemy, variant);
    }
    return enemy;
}

const char* EnemyFactory::getTypeName(int index) {
    if (index >= 0 && index < typeCount) return typeNames[index];
    return "Unknown";
}

int EnemyFactory::getTypeCount() { return typeCount; }

bool EnemyFactory::isBoss(const char* type) {
    return (strcmp(type, "Mogera") == 0 || strcmp(type, "Gamakichi") == 0);
}