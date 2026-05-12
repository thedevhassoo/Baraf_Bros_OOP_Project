#pragma once
#include <SFML/Graphics.hpp>

class Player;
class HUD;

// Types: Speed Boost, Snowball Power, Distance Increase, Balloon Mode
enum PowerUpType {
    POWERUP_SPEED,
    POWERUP_SNOWBALL,
    POWERUP_DISTANCE,
    POWERUP_BALLOON,
    POWERUP_EXTRA_LIFE,
    POWERUP_COUNT
};

class PowerUp {
public:
    PowerUpType type;
    float posX;
    float posY;
    float lifetime;
    bool active;
    sf::CircleShape visual;
    int starOptionIndex;    // For star bonus selection

    PowerUp();
    void spawn(float x, float y, PowerUpType t);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void collect(Player* player, HUD* hud);
    bool isActive() const;
    float getX() const;
    float getY() const;
};

class PowerUpSystem {
private:
    PowerUp** powerUps;
    int powerUpCount;
    int powerUpCapacity;

    // Star bonus
    bool starSpawned;
    bool starCollected;
    bool choosingStarPower;
    float starX;
    float starY;
    sf::CircleShape starVisual;
    PowerUpType starOptions[3];

    void pickRandomOptions();

public:
    PowerUpSystem();
    ~PowerUpSystem();

    void spawnPowerUp(float x, float y);
    void spawnStarBonus(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void checkCollection(Player* player1, Player* player2, HUD* hud);
    void clear();

    bool isChoosingStarPower() const;
    void selectStarPower(int option, Player* player, HUD* hud);
    PowerUpType getStarOption(int index) const;
};