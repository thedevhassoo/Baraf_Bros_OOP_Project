#pragma once
#include "Platform.h"
#include "EnemySpawnPoint.h"

class LevelData {
private:
    int levelNumber;
    char backgroundPath[128];
    char tileTexturePaths[3][128];

    Platform** platforms;
    int platformCount;
    int platformCapacity;

    EnemySpawnPoint* enemyHead;
    int enemyCount;

    float player1SpawnX;
    float player1SpawnY;
    float player2SpawnX;
    float player2SpawnY;

    void resizePlatforms();

public:
    LevelData();
    ~LevelData();

    // Getters
    int getLevelNumber() const;
    const char* getBackgroundPath() const;
    const char* getTileTexturePath(int index) const;
    int getPlatformCount() const;
    Platform* getPlatform(int index) const;
    EnemySpawnPoint* getEnemyHead() const;
    int getEnemyCount() const;
    float getPlayer1SpawnX() const;
    float getPlayer1SpawnY() const;
    float getPlayer2SpawnX() const;
    float getPlayer2SpawnY() const;

    // Setters
    void setLevelNumber(int n);
    void setBackgroundPath(const char* path);
    void setTileTexturePath(int index, const char* path);
    void setPlayer1Spawn(float x, float y);
    void setPlayer2Spawn(float x, float y);

    // Platform management
    void addPlatform(Platform* p);
    void removeLastPlatform();
    void clearPlatforms();

    // Enemy management
    void addEnemySpawn(const char* type, float x, float y, int variant);
    void removeLastEnemySpawn();
    void clearEnemySpawns();

    // File I/O
    bool saveToFile(const char* filename);
    bool loadFromFile(const char* filename);
};