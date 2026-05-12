#include "PowerUpSystem.h"
#include "Player.h"
#include "HUD.h"
#include <cstdlib>

PowerUp::PowerUp()
    : type(POWERUP_SPEED), posX(0), posY(0), lifetime(8.0f), active(false), starOptionIndex(-1) {
    visual.setRadius(12);
    visual.setOrigin(12, 12);
    visual.setOutlineThickness(2);
}

void PowerUp::spawn(float x, float y, PowerUpType t) {
    posX = x;
    posY = y;
    type = t;
    lifetime = 8.0f;
    active = true;
    starOptionIndex = -1;

    switch (t) {
    case POWERUP_SPEED:
        visual.setFillColor(sf::Color(50, 255, 50));
        visual.setOutlineColor(sf::Color::Green);
        break;
    case POWERUP_SNOWBALL:
        visual.setFillColor(sf::Color(100, 150, 255));
        visual.setOutlineColor(sf::Color::Blue);
        break;
    case POWERUP_DISTANCE:
        visual.setFillColor(sf::Color(255, 200, 50));
        visual.setOutlineColor(sf::Color::Yellow);
        break;
    case POWERUP_BALLOON:
        visual.setFillColor(sf::Color(255, 50, 255));
        visual.setOutlineColor(sf::Color::Magenta);
        break;
    case POWERUP_EXTRA_LIFE:
        visual.setFillColor(sf::Color(255, 50, 50));
        visual.setOutlineColor(sf::Color::Red);
        break;
    default: break;
    }
}

void PowerUp::update(float dt) {
    if (!active) return;
    lifetime -= dt;
    if (lifetime <= 0) active = false;
}

void PowerUp::draw(sf::RenderWindow& window) {
    if (!active) return;
    visual.setPosition(posX, posY);
    window.draw(visual);
}

void PowerUp::collect(Player* player, HUD* hud) {
    if (!active || !player) return;
    active = false;

    switch (type) {
    case POWERUP_SPEED:
        player->applySpeedBoost(15.0f);
        if (hud) hud->showPowerUp("SPEED BOOST", 3.0f);
        break;
    case POWERUP_SNOWBALL:
        player->applySnowballPower();
        if (hud) hud->showPowerUp("SNOWBALL POWER", 3.0f);
        break;
    case POWERUP_DISTANCE:
        player->applyDistanceIncrease();
        if (hud) hud->showPowerUp("RANGE UP", 3.0f);
        break;
    case POWERUP_BALLOON:
        player->applyBalloonMode(10.0f);
        if (hud) hud->showPowerUp("BALLOON", 3.0f);
        break;
    case POWERUP_EXTRA_LIFE:
        player->addLife();
        if (hud) hud->showPowerUp("+1 LIFE", 2.0f);
        break;
    default: break;
    }
}

bool PowerUp::isActive() const { return active; }
float PowerUp::getX() const { return posX; }
float PowerUp::getY() const { return posY; }

// PowerUpSystem

PowerUpSystem::PowerUpSystem()
    : powerUpCount(0), powerUpCapacity(8),
    starSpawned(false), starCollected(false), choosingStarPower(false),
    starX(0), starY(0) {

    powerUps = new PowerUp * [powerUpCapacity];
    for (int i = 0; i < powerUpCapacity; i++) powerUps[i] = nullptr;

    starVisual.setRadius(16);
    starVisual.setOrigin(16, 16);
    starVisual.setFillColor(sf::Color(255, 255, 50));
    starVisual.setOutlineColor(sf::Color(255, 200, 0));
    starVisual.setOutlineThickness(3);

    pickRandomOptions();
}

PowerUpSystem::~PowerUpSystem() {
    clear();
    delete[] powerUps;
}

void PowerUpSystem::pickRandomOptions() {
    starOptions[0] = (PowerUpType)(rand() % 4);
    starOptions[1] = (PowerUpType)(rand() % 4);
    starOptions[2] = (PowerUpType)(rand() % 4);
}

void PowerUpSystem::spawnPowerUp(float x, float y) {
    if (powerUpCount >= powerUpCapacity) return;

    PowerUp* pu = new PowerUp();
    PowerUpType type = (PowerUpType)(rand() % POWERUP_COUNT);
    pu->spawn(x, y, type);
    powerUps[powerUpCount++] = pu;
}

void PowerUpSystem::spawnStarBonus(float x, float y) {
    if (starSpawned) return;
    starX = x;
    starY = y;
    starSpawned = true;
    starCollected = false;
    pickRandomOptions();
}

void PowerUpSystem::update(float dt) {
    for (int i = 0; i < powerUpCount; i++) {
        if (powerUps[i]) {
            powerUps[i]->update(dt);
            if (!powerUps[i]->isActive()) {
                delete powerUps[i];
                for (int j = i; j < powerUpCount - 1; j++) {
                    powerUps[j] = powerUps[j + 1];
                }
                powerUps[powerUpCount - 1] = nullptr;
                powerUpCount--;
                i--;
            }
        }
    }
}

void PowerUpSystem::draw(sf::RenderWindow& window) {
    for (int i = 0; i < powerUpCount; i++) {
        if (powerUps[i] && powerUps[i]->isActive()) {
            powerUps[i]->draw(window);
        }
    }

    // Star bonus
    if (starSpawned && !starCollected) {
        starVisual.setPosition(starX, starY);
        window.draw(starVisual);
    }
}

void PowerUpSystem::checkCollection(Player* player1, Player* player2, HUD* hud) {
    Player* players[2] = { player1, player2 };

    for (int p = 0; p < 2; p++) {
        if (!players[p]) continue;

        float px = players[p]->getX();
        float py = players[p]->getY();
        float pw = 32;
        float ph = 48;

        // Check power-ups
        for (int i = 0; i < powerUpCount; i++) {
            if (!powerUps[i] || !powerUps[i]->isActive()) continue;
            float dx = px + pw / 2 - powerUps[i]->getX();
            float dy = py + ph / 2 - powerUps[i]->getY();
            if (dx * dx + dy * dy < 30 * 30) {
                powerUps[i]->collect(players[p], hud);
            }
        }

        // Check star
        if (starSpawned && !starCollected) {
            float dx = px + pw / 2 - starX;
            float dy = py + ph / 2 - starY;
            if (dx * dx + dy * dy < 35 * 35) {
                starCollected = true;
                choosingStarPower = true;
            }
        }
    }
}

bool PowerUpSystem::isChoosingStarPower() const { return choosingStarPower; }

void PowerUpSystem::selectStarPower(int option, Player* player, HUD* hud) {
    if (option < 0 || option > 2) return;

    PowerUp temp;
    temp.spawn(0, 0, starOptions[option]);
    temp.collect(player, hud);
    choosingStarPower = false;
}

PowerUpType PowerUpSystem::getStarOption(int index) const {
    return (index >= 0 && index < 3) ? starOptions[index] : POWERUP_SPEED;
}

void PowerUpSystem::clear() {
    for (int i = 0; i < powerUpCount; i++) delete powerUps[i];
    powerUpCount = 0;
    starSpawned = false;
    starCollected = false;
    choosingStarPower = false;
}