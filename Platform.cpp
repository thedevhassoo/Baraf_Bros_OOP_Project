#include "Platform.h"

Platform::Platform() : headTile(nullptr), tileCount(0) {}

Platform::~Platform() { clear(); }

void Platform::addTile(int type, float x, float y) {
    Tile* newTile = new Tile(type, x, y);
    if (!headTile) {
        headTile = newTile;
    }
    else {
        Tile* current = headTile;
        while (current->getNext()) {
            current = current->getNext();
        }
        current->setNext(newTile);
    }
    tileCount++;
}

Tile* Platform::getHeadTile() const { return headTile; }
int Platform::getTileCount() const { return tileCount; }

bool Platform::checkCollision(float px, float py, float pw, float ph, float& correctionY) const {
    Tile* current = headTile;
    while (current) {
        float tLeft = current->getX();
        float tRight = tLeft + current->getWidth();
        float tTop = current->getY();
        float tBottom = tTop + current->getHeight();

        // Check horizontal overlap
        if (px + pw > tLeft && px < tRight) {
            // Entity bottom is at or below platform top, and entity top is above platform top
            if (py + ph >= tTop && py < tTop) {
                correctionY = tTop - ph;
                return true;
            }
            // Entity top is at or above platform bottom (hitting head)
            if (py <= tBottom && py + ph > tBottom) {
                correctionY = tBottom;
                return true;
            }
        }
        current = current->getNext();
    }
    return false;
}

void Platform::clear() {
    Tile* current = headTile;
    while (current) {
        Tile* next = current->getNext();
        delete current;
        current = next;
    }
    headTile = nullptr;
    tileCount = 0;
}