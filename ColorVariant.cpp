#include "ColorVariant.h"
#include "Enemy.h"

void ColorVariant::applyVariant(Enemy* enemy, int variant) {
    if (!enemy || variant <= 0) return;

    float speedMult = getSpeedMultiplier(variant);
    int extraHits = getExtraHits(variant);

    enemy->setMaxHits(enemy->getMaxHits() + extraHits);
    enemy->setScoreValue((int)(enemy->getScoreValue() * speedMult));
    enemy->setGemDrop(enemy->getGemDrop() + variant);
    enemy->setColorVariant(variant);
}

float ColorVariant::getSpeedMultiplier(int variant) {
    switch (variant) {
    case 0: return 1.0f;
    case 1: return 1.25f;
    case 2: return 1.5f;
    default: return 1.0f;
    }
}

int ColorVariant::getExtraHits(int variant) {
    switch (variant) {
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    default: return 0;
    }
}

const char* ColorVariant::getVariantSuffix(int variant) {
    switch (variant) {
    case 0: return "_blue";
    case 1: return "_white";
    case 2: return "_black";
    default: return "_blue";
    }
}

int ColorVariant::getVariantCount() { return 3; }