#pragma once

class EnemySpawnPoint {
private:
    char enemyType[32];
    float posX;
    float posY;
    int colorVariant;       // 0=Red, 1=Green, 2=Blue
    EnemySpawnPoint* next;

public:
    EnemySpawnPoint();
    EnemySpawnPoint(const char* type, float x, float y, int variant);
    ~EnemySpawnPoint();

    const char* getEnemyType() const;
    float getX() const;
    float getY() const;
    int getColorVariant() const;
    EnemySpawnPoint* getNext() const;

    void setEnemyType(const char* type);
    void setPosition(float x, float y);
    void setColorVariant(int v);
    void setNext(EnemySpawnPoint* n);
};