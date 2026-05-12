#pragma once
#include "Tile.h"

class Platform {
private:
    Tile* headTile;
    int tileCount;

public:
    Platform();
    ~Platform();

    void addTile(int type, float x, float y);
    Tile* getHeadTile() const;
    int getTileCount() const;
    bool checkCollision(float px, float py, float pw, float ph, float& correctionY) const;
    void clear();
};