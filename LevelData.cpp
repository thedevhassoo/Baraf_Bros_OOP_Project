#include "LevelData.h"
#include <fstream>
#include <iostream>
#include <string>
LevelData::LevelData()
    : levelNumber(1), platformCount(0), platformCapacity(4),
    enemyHead(nullptr), enemyCount(0),
    player1SpawnX(100), player1SpawnY(500),
    player2SpawnX(200), player2SpawnY(500) {
    backgroundPath[0] = '\0';
    for (int i = 0; i < 3; i++) tileTexturePaths[i][0] = '\0';
    platforms = new Platform * [platformCapacity];
    for (int i = 0; i < platformCapacity; i++) platforms[i] = nullptr;
}

LevelData::~LevelData() {
    clearPlatforms();
    delete[] platforms;
    clearEnemySpawns();
}

void LevelData::resizePlatforms() {
    int newCap = platformCapacity * 2;
    Platform** newArr = new Platform * [newCap];
    for (int i = 0; i < platformCount; i++) newArr[i] = platforms[i];
    for (int i = platformCount; i < newCap; i++) newArr[i] = nullptr;
    delete[] platforms;
    platforms = newArr;
    platformCapacity = newCap;
}

int LevelData::getLevelNumber() const { return levelNumber; }
const char* LevelData::getBackgroundPath() const { return backgroundPath; }
const char* LevelData::getTileTexturePath(int index) const {
    return (index >= 0 && index < 3) ? tileTexturePaths[index] : "";
}
int LevelData::getPlatformCount() const { return platformCount; }
Platform* LevelData::getPlatform(int index) const {
    return (index >= 0 && index < platformCount) ? platforms[index] : nullptr;
}
EnemySpawnPoint* LevelData::getEnemyHead() const { return enemyHead; }
int LevelData::getEnemyCount() const { return enemyCount; }
float LevelData::getPlayer1SpawnX() const { return player1SpawnX; }
float LevelData::getPlayer1SpawnY() const { return player1SpawnY; }
float LevelData::getPlayer2SpawnX() const { return player2SpawnX; }
float LevelData::getPlayer2SpawnY() const { return player2SpawnY; }

void LevelData::setLevelNumber(int n) { levelNumber = n; }

void LevelData::setBackgroundPath(const char* path) {
    int i = 0;
    while (path[i] && i < 127) { backgroundPath[i] = path[i]; i++; }
    backgroundPath[i] = '\0';
}

void LevelData::setTileTexturePath(int index, const char* path) {
    if (index < 0 || index >= 3) return;
    int i = 0;
    while (path[i] && i < 127) { tileTexturePaths[index][i] = path[i]; i++; }
    tileTexturePaths[index][i] = '\0';
}

void LevelData::setPlayer1Spawn(float x, float y) { player1SpawnX = x; player1SpawnY = y; }
void LevelData::setPlayer2Spawn(float x, float y) { player2SpawnX = x; player2SpawnY = y; }

void LevelData::addPlatform(Platform* p) {
    if (platformCount >= platformCapacity) resizePlatforms();
    platforms[platformCount++] = p;
}

void LevelData::removeLastPlatform() {
    if (platformCount > 0) {
        platformCount--;
        delete platforms[platformCount];
        platforms[platformCount] = nullptr;
    }
}

void LevelData::clearPlatforms() {
    for (int i = 0; i < platformCount; i++) {
        delete platforms[i];
        platforms[i] = nullptr;
    }
    platformCount = 0;
}

void LevelData::addEnemySpawn(const char* type, float x, float y, int variant) {
    EnemySpawnPoint* e = new EnemySpawnPoint(type, x, y, variant);
    if (!enemyHead) {
        enemyHead = e;
    }
    else {
        EnemySpawnPoint* cur = enemyHead;
        while (cur->getNext()) cur = cur->getNext();
        cur->setNext(e);
    }
    enemyCount++;
}

void LevelData::removeLastEnemySpawn() {
    if (!enemyHead) return;
    if (!enemyHead->getNext()) {
        delete enemyHead;
        enemyHead = nullptr;
        enemyCount = 0;
        return;
    }
    EnemySpawnPoint* cur = enemyHead;
    while (cur->getNext() && cur->getNext()->getNext()) cur = cur->getNext();
    delete cur->getNext();
    cur->setNext(nullptr);
    enemyCount--;
}

void LevelData::clearEnemySpawns() {
    EnemySpawnPoint* cur = enemyHead;
    while (cur) {
        EnemySpawnPoint* next = cur->getNext();
        delete cur;
        cur = next;
    }
    enemyHead = nullptr;
    enemyCount = 0;
}

bool LevelData::saveToFile(const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << levelNumber << "\n";
    file << player1SpawnX << " " << player1SpawnY << "\n";
    file << player2SpawnX << " " << player2SpawnY << "\n";

    file << platformCount << "\n";
    for (int i = 0; i < platformCount; i++) {
        Platform* p = platforms[i];
        if (!p) continue;
        file << p->getTileCount() << "\n";
        Tile* t = p->getHeadTile();
        while (t) {
            file << t->getTileType() << " " << t->getX() << " " << t->getY() << "\n";
            t = t->getNext();
        }
    }

    file << enemyCount << "\n";
    EnemySpawnPoint* e = enemyHead;
    while (e) {
        file << e->getEnemyType() << " " << e->getX() << " " << e->getY() << " " << e->getColorVariant() << "\n";
        e = e->getNext();
    }

    file.close();
    return true;
}

bool LevelData::loadFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    clearPlatforms();
    clearEnemySpawns();

    file >> levelNumber;
    file.ignore();
    //file.getline(backgroundPath, 128);

    // If backgroundPath is empty or old format, regenerate
    //if (backgroundPath[0] == '\0' || backgroundPath[0] == '.') {
   std::string bgPath = "./assets/levels/level" + std::to_string(levelNumber) + "/bg_level" + std::to_string(levelNumber) + ".png";
    int i = 0;
    while (bgPath[i] != '\0' && i < 127) {
        backgroundPath[i] = bgPath[i];
        i++;
    }
    backgroundPath[i] = '\0';
    //}

    file >> player1SpawnX >> player1SpawnY;
    file >> player2SpawnX >> player2SpawnY;

    int pCount;
    file >> pCount;
    for (int i = 0; i < pCount; i++) {
        Platform* p = new Platform();
        int tCount;
        file >> tCount;
        for (int j = 0; j < tCount; j++) {
            int type;
            float x, y;
            file >> type >> x >> y;
            p->addTile(type, x, y);
        }
        addPlatform(p);
    }

    int eCount;
    file >> eCount;
    for (int i = 0; i < eCount; i++) {
        char type[32];
        float x, y;
        int variant;
        file >> type >> x >> y >> variant;
        addEnemySpawn(type, x, y, variant);
    }

    file.close();

    std::cout << "Loaded level " << levelNumber << ": "
        << platformCount << " platforms, "
        << enemyCount << " enemies" << std::endl;
    return true;
}