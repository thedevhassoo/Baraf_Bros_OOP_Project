#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Snowball.h"
#include "HUD.h"
#include "PowerUpSystem.h"
#include "ShopSystem.h"
#include "SaveManager.h"
#include <SFML/Audio.hpp>
class LevelLoader;
class LevelData;
class Platform;
class Mogera;
class Gamakichi;
class Tornado;
class Knife;
class ArtilleryRocket;

class GameState : public Scene {
private:
    sf::Music menuMusic;
    sf::Music levelMusic;
    int currentMusicLevel;
    // Level
    LevelLoader* levelLoader;
    LevelData* currentLevel;
    int currentLevelNumber;
    int totalLevels;
    char levelColorVariant;     // 0, 1, or 2 based on level range
    int startLevel;
    // Background
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    static const int MAX_GLOBAL_TILES = 18;
    sf::Texture globalTileTex[MAX_GLOBAL_TILES];
    sf::Sprite globalTileSpr[MAX_GLOBAL_TILES];
    bool globalTilesLoaded;
    int storedSpeedBoosts;
    int storedSnowballPowers;
    int storedRangeBoosts;
    int storedBalloons;
    int storedExtraLives;
    // Players
    Player* player1;
    Player* player2;
    int playerCount;
    sf::Texture player1Texture;
    sf::Texture player2Texture;
    int p1CharIndex;
    int p2CharIndex;

    // Enemies
    Enemy** enemies;
    int enemyCount;
    int enemyCapacity;

    // Snowballs
    Snowball** snowballs;
    int snowballCount;
    int maxSnowballs;

    // Boss references (for health bars and special handling)
    Mogera* activeMogera;
    Gamakichi* activeGamakichi;

    // Systems
    HUD hud;
    PowerUpSystem powerUpSystem;
    ShopSystem shopSystem;
    SaveManager saveManager;

    // Game state
    int score;
    int gems;
    int chainKillCount;
    bool isPaused;
    bool isGameOver;
    bool isLevelComplete;
    float levelCompleteTimer;
    bool showHitboxes;
    bool isBonusLevel;
    float bonusRainTimer;
    int bonusItemsCollected;
    int bonusItemsMax;
    float* bonusItemX;
    float* bonusItemY;
    bool* bonusItemActive;

    // Star bonus
    bool starBonusLevel;
    bool starSpawnedThisLevel;


    // Methods
    void loadLevel(int levelNum);
    void spawnEnemies();
    void clearEnemies();
    void clearSnowballs();
    void addEnemy(Enemy* e);
    void removeEnemy(int index);
    void addSnowball(Snowball* s);
    void removeSnowball(int index);
    void resizeEnemyArray();
	void loadGlobalTiles();

    // Collisions
    void checkAllCollisions();
    void checkSnowballEnemyCollisions();
    void checkPlayerEnemyCollisions();
    void checkRollingEnemyCollisions();
    void checkPlayerKnifeCollisions();
    void checkPlayerRocketCollisions();
    void checkBossChildrenSnowballCollisions();

    // Level flow
    void checkLevelComplete();
    void levelComplete();
    void loadNextLevel();
    void gameOver();

    // Bonus level
    void spawnBonusRain();
    void updateBonusRain(float dt);
    void drawBonusRain(sf::RenderWindow& window);

    // Input
    void handleGameInput(const sf::Event& event);
    void processPlayerThrows();

    // Boss helpers
    void updateBossHealthBars();
    int getCurrentColorVariant() const;

    void playLevelMusic(int levelNum);

    sf::Texture pauseTexture;
    sf::Sprite pauseSprite;
    sf::Texture gemTexture;
    sf::Sprite gemSprite;
    bool gemTextureLoaded;
    float bossDeathDelay;

public:
    GameState();
    ~GameState();
    void setStartLevel(int l);
    void setLevelLoader(LevelLoader* loader);
    void setPlayerMode(int mode);
    void init(LevelLoader* loader, int playerMode);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
    void onEnter() override;
    void onExit() override;
    void setCharacterSelection(int p1, int p2);
    void checkPlayerBossChildCollisions();
};