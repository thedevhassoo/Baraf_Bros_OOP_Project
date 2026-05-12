#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
LevelLoader::LevelLoader() : levelCache(nullptr), cacheSize(0), totalLevels(10) {}

LevelLoader::~LevelLoader() { clear(); }

int LevelLoader::readLevelCount(const char* configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) return 10;

    char line[128];
    while (file.getline(line, 128)) {
        if (strncmp(line, "levelCount", 10) == 0) {
            char* eq = nullptr;
            for (int i = 0; line[i]; i++) {
                if (line[i] == '=') { eq = &line[i]; break; }
            }
            if (eq) {
                eq++;
                while (*eq == ' ' || *eq == '\t') eq++;
                int count = 0;
                while (*eq >= '0' && *eq <= '9') {
                    count = count * 10 + (*eq - '0');
                    eq++;
                }
                file.close();
                return count;
            }
        }
    }
    file.close();
    return 10;
}

bool LevelLoader::init(const char* configPath) {
    totalLevels = readLevelCount(configPath);
    cacheSize = totalLevels;
    levelCache = new LevelData * [cacheSize];

    for (int i = 0; i < cacheSize; i++) {
        levelCache[i] = new LevelData();
        std::string filename = "./assets/levels/level" + std::to_string(i + 1) + "/level" + std::to_string(i + 1) + ".txt";
        std::ifstream test(filename);
        if (test.is_open()) {
            test.close();
            levelCache[i]->loadFromFile(filename.c_str());
        }
    }
    return true;
}

bool LevelLoader::reloadLevel(int levelNumber) {
    int index = levelNumber - 1;
    if (index >= 0 && index < cacheSize) {
        std::string filename = "./assets/levels/level" + std::to_string(levelNumber) + "/level" + std::to_string(levelNumber) + ".txt";
        return levelCache[index]->loadFromFile(filename.c_str());
    }
    return false;
}
LevelData* LevelLoader::getLevel(int levelNumber) {
    int index = levelNumber - 1;
    return (index >= 0 && index < cacheSize) ? levelCache[index] : nullptr;
}

int LevelLoader::getTotalLevels() const { return totalLevels; }


void LevelLoader::clear() {
    if (levelCache) {
        for (int i = 0; i < cacheSize; i++) delete levelCache[i];
        delete[] levelCache;
        levelCache = nullptr;
    }
    cacheSize = 0;
}