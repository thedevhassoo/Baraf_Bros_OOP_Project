#pragma once

class Enemy;

class EnemyFactory {
private:
    static const char* typeNames[];
    static const int typeCount;

public:
    // Create enemy by name, returns nullptr if unknown
    static Enemy* createEnemy(const char* type);

    // Create with color variant applied
    static Enemy* createEnemyWithVariant(const char* type, int variant);

    // Get name of enemy type by index
    static const char* getTypeName(int index);

    // Total registered types (excluding color variants, they're just stat mods)
    static int getTypeCount();

    // Check if a type is a boss
    static bool isBoss(const char* type);
};