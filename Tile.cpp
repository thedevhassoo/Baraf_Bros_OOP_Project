#include "Tile.h"

const float Tile::TILE_SIZE = 64.0f;

Tile::Tile() : tileType(0), posX(0), posY(0), next(nullptr) {}

Tile::Tile(int type, float x, float y)
    : tileType(type), posX(x), posY(y), next(nullptr) {
}

Tile::~Tile() {}

int Tile::getTileType() const { return tileType; }
float Tile::getX() const { return posX; }
float Tile::getY() const { return posY; }
float Tile::getWidth() const { return TILE_SIZE; }
float Tile::getHeight() const { return TILE_SIZE; }
Tile* Tile::getNext() const { return next; }

void Tile::setTileType(int type) { tileType = type; }
void Tile::setPosition(float x, float y) { posX = x; posY = y; }
void Tile::setNext(Tile* n) { next = n; }