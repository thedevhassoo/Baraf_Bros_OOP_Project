#include "EnemySpawnPoint.h"
#include <cstring>

EnemySpawnPoint::EnemySpawnPoint() : posX(0), posY(0), colorVariant(0), next(nullptr) {
    enemyType[0] = '\0';
}

EnemySpawnPoint::EnemySpawnPoint(const char* type, float x, float y, int variant)
    : posX(x), posY(y), colorVariant(variant), next(nullptr) {
    int i = 0;
    while (type[i] != '\0' && i < 31) {
        enemyType[i] = type[i];
        i++;
    }
    enemyType[i] = '\0';
}

EnemySpawnPoint::~EnemySpawnPoint() {}

const char* EnemySpawnPoint::getEnemyType() const { return enemyType; }
float EnemySpawnPoint::getX() const { return posX; }
float EnemySpawnPoint::getY() const { return posY; }
int EnemySpawnPoint::getColorVariant() const { return colorVariant; }
EnemySpawnPoint* EnemySpawnPoint::getNext() const { return next; }

void EnemySpawnPoint::setEnemyType(const char* type) {
    int i = 0;
    while (type[i] != '\0' && i < 31) { enemyType[i] = type[i]; i++; }
    enemyType[i] = '\0';
}

void EnemySpawnPoint::setPosition(float x, float y) { posX = x; posY = y; }
void EnemySpawnPoint::setColorVariant(int v) { colorVariant = v; }
void EnemySpawnPoint::setNext(EnemySpawnPoint* n) { next = n; }