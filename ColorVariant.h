#pragma once

class Enemy;

class ColorVariant {
public:
    // Apply variant stats to enemy (health, speed, score, gem drop)
    static void applyVariant(Enemy* enemy, int variant);

    // Get stat modifiers by variant
    static float getSpeedMultiplier(int variant);
    static int getExtraHits(int variant);

    // Texture suffix: 0="_red", 1="_green", 2="_blue"
    static const char* getVariantSuffix(int variant);

    // Variant count
    static int getVariantCount();
};