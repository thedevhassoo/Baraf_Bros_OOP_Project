#pragma once
#include "LevelData.h"

class LevelLoader {
private:
    LevelData** levelCache;
    int cacheSize;
    int totalLevels;

    int readLevelCount(const char* configPath);

public:
    LevelLoader();
    ~LevelLoader();

    bool init(const char* configPath);
    LevelData* getLevel(int levelNumber);
    int getTotalLevels() const;
    bool reloadLevel(int levelNumber);
    void clear();
};