#pragma once

class Tile {
private:
    int tileType;       // 0, 1, or 2 (visual variant)
    float posX;         // Grid-snapped X (multiple of 64)
    float posY;         // Grid-snapped Y (multiple of 64)
    Tile* next;         // Linked list within a platform

    static const float TILE_SIZE;

public:
    Tile();
    Tile(int type, float x, float y);
    ~Tile();

    int getTileType() const;
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    Tile* getNext() const;

    void setTileType(int type);
    void setPosition(float x, float y);
    void setNext(Tile* n);
};