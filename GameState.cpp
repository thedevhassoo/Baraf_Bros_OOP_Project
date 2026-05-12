#include "GameState.h"
#include "LevelLoader.h"
#include "LevelData.h"
#include "Platform.h"
#include "Tile.h"
#include "EnemyFactory.h"
#include "ColorVariant.h"
#include "Mogera.h"
#include "Gamakichi.h"
#include "Tornado.h"
#include "Knife.h"
#include "ArtilleryRocket.h"
#include "Snowball.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "AuthManager.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "MenuScene.h"

static bool isPlayerAlive(Player* player) {
    return player && player->getLives() > 0;
}

static void respawnPlayerIfAlive(Player* player, float spawnX, float spawnY) {
    if (!isPlayerAlive(player)) {
        if (player) player->setVelocity(0, 0);
        return;
    }

    player->setPosition(spawnX, spawnY);
    player->setVelocity(0, 0);
}
GameState::GameState()
    : levelLoader(nullptr), currentLevel(nullptr), currentLevelNumber(1), totalLevels(10),
    levelColorVariant(0),
    globalTilesLoaded(false),
    player1(nullptr), player2(nullptr), playerCount(1),
    enemies(nullptr), enemyCount(0), enemyCapacity(10),
    snowballs(nullptr), snowballCount(0), maxSnowballs(3),
    activeMogera(nullptr), activeGamakichi(nullptr),
    score(0), gems(0), chainKillCount(0),
    isPaused(false), isGameOver(false), isLevelComplete(false),
    levelCompleteTimer(0), showHitboxes(false),
    isBonusLevel(false), bonusRainTimer(0), bonusItemsCollected(0), bonusItemsMax(10),
    bonusItemX(nullptr), bonusItemY(nullptr), bonusItemActive(nullptr),
    starBonusLevel(false), starSpawnedThisLevel(false), currentMusicLevel(-1)
{
    storedSpeedBoosts = 0;
    storedSnowballPowers = 0;
    storedRangeBoosts = 0;
    storedBalloons = 0;
    storedExtraLives = 0;
    startLevel = 1;
    p1CharIndex = 0; // Default to first character
    p2CharIndex = 0; // Default to first character
    pauseTexture.loadFromFile("./assets/gameplay/pause.png");
    pauseSprite.setTexture(pauseTexture);
    enemies = new Enemy * [enemyCapacity];
    for (int i = 0; i < enemyCapacity; i++) enemies[i] = nullptr;

    snowballs = new Snowball * [maxSnowballs + 1];
    for (int i = 0; i <= maxSnowballs; i++) snowballs[i] = nullptr;

    bonusItemX = new float[bonusItemsMax];
    bonusItemY = new float[bonusItemsMax];
    bonusItemActive = new bool[bonusItemsMax];
    for (int i = 0; i < bonusItemsMax; i++) bonusItemActive[i] = false;

}

GameState::~GameState() {
    clearEnemies();
    delete[] enemies;
    clearSnowballs();
    delete[] snowballs;
    if (player1) delete player1;
    if (player2) delete player2;
    delete[] bonusItemX;
    delete[] bonusItemY;
    delete[] bonusItemActive;
}
void GameState::playLevelMusic(int levelNum) {
    if (currentMusicLevel == levelNum) return;

    if (levelMusic.getStatus() == sf::Music::Playing) {
        levelMusic.stop();
    }

    currentMusicLevel = levelNum;
    const char* path = nullptr;
    //if(activeGamakichi || activeMogera) path = "./assets/audio/bossfinal.wav";
    if (levelNum >= 1 && levelNum <= 3)  path = "./assets/audio/basic.wav";
    else if (levelNum == 4) path = "./assets/audio/bonus.wav";
    else if (levelNum == 5) path = "./assets/audio/boss.wav";
    else if (levelNum >= 6 && levelNum <= 8) path = "./assets/audio/advance.wav";
    else if (levelNum == 9) path = "./assets/audio/bonus.wav";
    else if (levelNum == 10)path = "./assets/audio/finalboss.wav";

    if (path && levelMusic.openFromFile(path)) {
        levelMusic.setLoop(true);
        levelMusic.setVolume(70);
        levelMusic.play();
    }
}
void GameState::init(LevelLoader* loader, int playerMode) {
    levelLoader = loader;
    playerCount = playerMode;
    totalLevels = loader->getTotalLevels();
}

int GameState::getCurrentColorVariant() const {
    // Spec 6.4: Levels 1-5 = Red (0), 6-7 = Green (1), 8-10 = Blue (2)
    if (currentLevelNumber <= 5) return 0;
    if (currentLevelNumber <= 7) return 1;
    return 2;
}

void GameState::loadLevel(int levelNum) {
    std::cout << "Loading level " << levelNum << std::endl;
    currentLevelNumber = levelNum;
    currentLevel = levelLoader->getLevel(levelNum);
    if (!currentLevel) return;

    playLevelMusic(levelNum);

    levelColorVariant = getCurrentColorVariant();

    // Load background
    const char* bgPath = currentLevel->getBackgroundPath();
    if (bgPath[0] != '\0') {
        bgTexture.loadFromFile(bgPath);
    }
    else {
        std::string path = "./assets/levels/level" + std::to_string(levelNum) + "/bg_level" + std::to_string(levelNum) + ".png";
        bgTexture.loadFromFile(path);
    }
    bgSprite.setTexture(bgTexture);

    // Position players
    player1->setPosition(currentLevel->getPlayer1SpawnX(), currentLevel->getPlayer1SpawnY());
    player1->setVelocity(0, 0);
    player1->setGrounded(false);
    if (player2) {
        player2->setPosition(currentLevel->getPlayer2SpawnX(), currentLevel->getPlayer2SpawnY());
        player2->setVelocity(0, 0);
        player2->setGrounded(false);
    }
    player1->clearPowerUps();
    if (player2) player2->clearPowerUps();
    // FIXED bonus levels: 4 and 9
    isBonusLevel = (levelNum == 4 || levelNum == 9);
    starBonusLevel = false;
    starSpawnedThisLevel = false;
    if (isBonusLevel) {
        levelCompleteTimer = 5.0f;
    }
    else {
        levelCompleteTimer = 2.5f;
    }

    clearEnemies();
    clearSnowballs();
    powerUpSystem.clear();
    activeMogera = nullptr;
    activeGamakichi = nullptr;
    spawnEnemies();


    hud.setLevel(levelNum, totalLevels);
    hud.hideBossHealth();
    hud.hideOverlay();

    loadGlobalTiles();
}

void GameState::spawnEnemies() {
    if (!currentLevel) return;

    EnemySpawnPoint* spawn = currentLevel->getEnemyHead();
    while (spawn) {
        const char* type = spawn->getEnemyType();
        int variant = spawn->getColorVariant();

        // For non-boss enemies, use the placed variant (not auto color variant)
        // Bosses ignore variant
        if (EnemyFactory::isBoss(type)) {
            variant = 0;  // Bosses always base color
        }

        Enemy* enemy = EnemyFactory::createEnemyWithVariant(type, variant);
        if (enemy) {

            // v=0 Blue, v=1 White, v=2 Black
            if (variant == 0) enemy->speedMultiplier = 1.0f;       // Blue - normal
            else if (variant == 1) enemy->speedMultiplier = 1.25f;  // White - fast
            else if (variant == 2) enemy->speedMultiplier = 1.5f;   // Black - fastest
            enemy->setPosition(spawn->getX(), spawn->getY());

            // Load textures
            if (strcmp(type, "Botom") == 0) {
                const char* paths[3] = {
                    "./assets/enemies/botom_white.png",
                    "./assets/enemies/botom_blue.png",
                    "./assets/enemies/botom_black.png"
                };

                int frameSizes[3][2] = {
                    {120, 104},
                    {121, 111},
                    {129, 109}
                };

                int v = enemy->getColorVariant();
                if (v < 0) v = 0;
                if (v > 2) v = 2;

                Spritesheet* sheet = new Spritesheet(3, paths[v], { frameSizes[v][0], frameSizes[v][1] });
                sheet->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(frameSizes[v][0], frameSizes[v][1]), 1);
                sheet->createAnim(1, 3, sf::Vector2i(frameSizes[v][0], 0), sf::Vector2i(frameSizes[v][0], frameSizes[v][1]), 8);
                sheet->createAnim(2, 3, sf::Vector2i(0, frameSizes[v][1]), sf::Vector2i(frameSizes[v][0], frameSizes[v][1]), 6);
                sheet->setAnimLoop(0, true);
                sheet->setAnimLoop(1, true);
                sheet->setAnimLoop(2, false);

                sheet->setSpriteOrigin(frameSizes[v][0] / 2.0f, frameSizes[v][1] / 2.0f);
                enemy->setEnemySpritesheet(sheet);
                enemy->configureEnemySprite(frameSizes[v][0], frameSizes[v][1], 70, 55);
                enemy->setHitboxOffset(0, -15);
            }
            else if (strcmp(type, "FlyingFoogaFoog") == 0) {
                const char* paths[3] = {
        "./assets/enemies/fooga_white.png",
        "./assets/enemies/fooga_blue.png",
        "./assets/enemies/fooga_black.png"
                };
                int fw[3] = { 135, 126, 133 };
                int fh[3] = { 140, 135, 131 };
                int v = enemy->getColorVariant(); if (v < 0) v = 0; if (v > 2) v = 2;

                Spritesheet* s = new Spritesheet(3, paths[v], { fw[v], fh[v] });

                // Anim 0: Fly/Move — row 0, frames 1, 2, 3
                s->createAnim(0, 3, sf::Vector2i(0, 0), sf::Vector2i(fw[v], fh[v]), 10);
                // Anim 1: Hit — row 1, frames 5, 6
                s->createAnim(2, 2, sf::Vector2i(0, fh[v]), sf::Vector2i(fw[v], fh[v]), 6);
                s->createAnim(1, 1, sf::Vector2i(fw[v] * 3, 0), sf::Vector2i(fw[v], fh[v]), 1);
                s->setAnimLoop(0, true);
                s->setAnimLoop(1, false);
                s->setSpriteOrigin(fw[v] / 2.0f, fh[v] / 2.0f);
                enemy->setEnemySpritesheet(s);
                enemy->configureEnemySprite(fw[v], fh[v], 70, 70);
                enemy->setHitboxOffset(0, -15);
            }
            else if (strcmp(type, "Tornado") == 0) {
                ((Tornado*)enemy)->setKnifeSound("./assets/audio/knife.wav");
                const char* paths[3] = {
                    "./assets/enemies/tornado_white.png",
                    "./assets/enemies/tornado_blue.png",
                    "./assets/enemies/tornado_black.png"
                };
                int fw[3] = { 136, 131, 125 };
                int fh[3] = { 89, 91, 96 };
                int v = enemy->getColorVariant(); if (v < 0) v = 0; if (v > 2) v = 2;

                Spritesheet* s = new Spritesheet(3, paths[v], { fw[v], fh[v] });

                s->createAnim(0, 4, sf::Vector2i(0, 0), sf::Vector2i(fw[v], fh[v]), 10);
                s->createAnim(1, 4, sf::Vector2i(0, fh[v]), sf::Vector2i(fw[v], fh[v]), 10);
                s->createAnim(2, 3, sf::Vector2i(0, fh[v] * 2), sf::Vector2i(fw[v], fh[v]), 6);

                s->setAnimLoop(0, true);
                s->setAnimLoop(1, false);
                s->setAnimLoop(2, false);
                s->setSpriteOrigin(fw[v] / 2.0f, fh[v]);
                enemy->setEnemySpritesheet(s);
                enemy->configureEnemySprite(fw[v], fh[v], 60, 70);
                enemy->setHitboxOffset(-5, -15);
            }
            // Track bosses
            else if (strcmp(type, "Mogera") == 0) {
                activeMogera = (Mogera*)enemy;
                Spritesheet* s = new Spritesheet(3, "./assets/enemies/mogera.png", { 281, 273 });
                s->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(281, 273), 10);
                // Anim 0: Spawn — row 1+2, 8 frames
                s->createAnim(1, 8, sf::Vector2i(0, 0), sf::Vector2i(281, 273), 10);
                // Anim 1: Die — row 3+4, 7 frames
                s->createAnim(2, 7, sf::Vector2i(0, 546), sf::Vector2i(281, 273), 6);

                s->setAnimLoop(0, true);
                s->setAnimLoop(1, false);
                s->setAnimLoop(2, false);
                s->setSpriteOrigin(281 / 2.0f, 273);
                enemy->setEnemySpritesheet(s);
                enemy->configureEnemySprite(281, 273, 181, 203);
            }
            else if (strcmp(type, "Gamakichi") == 0)
            {
                activeGamakichi = (Gamakichi*)enemy;

                Spritesheet* s = new Spritesheet(3, "./assets/enemies/gamakichi.png", { 196, 177 });

                // Anim 0: Rocket — row 0, frames 1-4
                s->createAnim(0, 4, sf::Vector2i(0, 0), sf::Vector2i(196, 177), 10);
                // Anim 1: Idle — row 1, frames 1-4
                s->createAnim(1, 4, sf::Vector2i(0, 177), sf::Vector2i(196, 177), 8);
                // Anim 2: Die — row 2, frames 1-3
                s->createAnim(2, 3, sf::Vector2i(0, 354), sf::Vector2i(196, 177), 6);

                s->setAnimLoop(0, false);
                s->setAnimLoop(1, true);
                s->setAnimLoop(2, false);
                s->setSpriteOrigin(196 / 2.0f, 177);  // Bottom-center
                enemy->setEnemySpritesheet(s);
                enemy->configureEnemySprite(196, 177, 197, 191);
            }

            addEnemy(enemy);
        }
        spawn = spawn->getNext();
    }
}
void GameState::checkPlayerBossChildCollisions() {
    Player* players[2] = { player1, player2 };

    if (activeMogera) {
        Botom** children = activeMogera->getChildren();
        int childCount = activeMogera->getChildCount();

        for (int c = 0; c < childCount; c++) {
            if (!children[c]) continue;

            HitBox* childHit = children[c]->getHitbox();
            if (!childHit) continue;

            for (int p = 0; p < playerCount; p++) {
                if (!isPlayerAlive(players[p]) || players[p]->isDying) continue;

                HitBox* playerHit = players[p]->getHitbox();
                if (!playerHit) continue;

                if (playerHit->intersects(*childHit)) {
                    if (children[c]->getIsEncased() && !children[c]->getIsRolling()) {
                        float dir = players[p]->getFacingRight() ? 1.0f : -1.0f;
                        children[c]->startRoll(dir);
                    }
                    else if (!children[c]->getIsEncased()) {
                        players[p]->takeDamage();
                        hud.setLivesP1(player1->getLives());
                        if (player2) hud.setLivesP2(player2->getLives());

                        float sx = (p == 0)
                            ? currentLevel->getPlayer1SpawnX()
                            : currentLevel->getPlayer2SpawnX();

                        float sy = (p == 0)
                            ? currentLevel->getPlayer1SpawnY()
                            : currentLevel->getPlayer2SpawnY();

                        respawnPlayerIfAlive(players[p], sx, sy);

                        if (!isPlayerAlive(players[p])) continue;
                    }
                }
            }
        }
    }
}
void GameState::loadGlobalTiles() {
    if (globalTilesLoaded) return;

    for (int i = 0; i < MAX_GLOBAL_TILES; i++) {
        std::string path = "./assets/tiles/tile" + std::to_string(i) + ".png";
        if (globalTileTex[i].loadFromFile(path)) {
            globalTileSpr[i].setTexture(globalTileTex[i]);
        }
    }
    globalTilesLoaded = true;
}
void GameState::addEnemy(Enemy* e) {
    if (enemyCount >= enemyCapacity) resizeEnemyArray();
    enemies[enemyCount++] = e;
}

void GameState::removeEnemy(int index) {
    if (index < 0 || index >= enemyCount) return;
    if (enemies[index] == activeMogera) activeMogera = nullptr;
    if (enemies[index] == activeGamakichi) activeGamakichi = nullptr;
    delete enemies[index];
    for (int i = index; i < enemyCount - 1; i++) {
        enemies[i] = enemies[i + 1];
    }
    enemies[enemyCount - 1] = nullptr;
    enemyCount--;
}

void GameState::resizeEnemyArray() {
    int newCap = enemyCapacity * 2;
    Enemy** newArr = new Enemy * [newCap];
    for (int i = 0; i < enemyCount; i++) newArr[i] = enemies[i];
    for (int i = enemyCount; i < newCap; i++) newArr[i] = nullptr;
    delete[] enemies;
    enemies = newArr;
    enemyCapacity = newCap;
}

void GameState::clearEnemies() {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i]) delete enemies[i];
    }
    enemyCount = 0;
    for (int i = 0; i < enemyCapacity; i++) enemies[i] = nullptr;
    activeMogera = nullptr;
    activeGamakichi = nullptr;
}

void GameState::addSnowball(Snowball* s) {
    if (snowballCount >= maxSnowballs) {
        delete s;
        return;
    }
    snowballs[snowballCount++] = s;
}

void GameState::removeSnowball(int index) {
    if (index < 0 || index >= snowballCount) return;
    delete snowballs[index];
    for (int i = index; i < snowballCount - 1; i++) {
        snowballs[i] = snowballs[i + 1];
    }
    snowballs[snowballCount - 1] = nullptr;
    snowballCount--;
}

void GameState::clearSnowballs() {
    for (int i = 0; i < snowballCount; i++) delete snowballs[i];
    snowballCount = 0;
}

void GameState::handleGameInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        AuthManager* auth = nullptr;
        if (sceneManager && sceneManager->gameManager) {
            auth = sceneManager->gameManager->authManager;
        }

        if (event.key.code == sf::Keyboard::F1 || event.key.code == sf::Keyboard::H) {
            showHitboxes = !showHitboxes;
        }

        // Power-ups for BOTH players at once
        if (event.key.code == sf::Keyboard::B && storedSpeedBoosts > 0) {
            player1->applySpeedBoost(30.0f);
            if (player2) player2->applySpeedBoost(30.0f);
            storedSpeedBoosts--;
            if (auth) auth->updateSpeedBoosts(storedSpeedBoosts);
        }
        if (event.key.code == sf::Keyboard::N && storedSnowballPowers > 0) {
            player1->applySnowballPower();
            if (player2) player2->applySnowballPower();
            storedSnowballPowers--;
            if (auth) auth->updateBalloons(storedSnowballPowers);
        }
        if (event.key.code == sf::Keyboard::M && storedRangeBoosts > 0) {
            player1->applyDistanceIncrease();
            if (player2) player2->applyDistanceIncrease();
            storedRangeBoosts--;
            if (auth) auth->updateDistancePUP(storedRangeBoosts);
        }
        if (event.key.code == sf::Keyboard::V && storedBalloons > 0) {
            player1->applyBalloonMode(30.0f);
            if (player2) player2->applyBalloonMode(30.0f);
            storedBalloons--;
        }
        if (event.key.code == sf::Keyboard::C && storedExtraLives > 0) {
            player1->addLife();
            if (player2) player2->addLife();
            storedExtraLives--;
            if (auth) auth->updateSecondChanceChips(storedExtraLives);
        }

        if (event.key.code == sf::Keyboard::P) {
            isPaused = !isPaused;
        }
        if (event.key.code == sf::Keyboard::Escape) {
            if (isPaused || isGameOver) {
                if (sceneManager) {
                    sceneManager->changeScene(new MenuScene());
                }
            }
            else {
                isPaused = !isPaused;
            }
        }
    }

}

void GameState::processPlayerThrows() {

    // Player 1
    // Player 1
    if (isPlayerAlive(player1)) {
        bool throwKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        if (throwKey && player1->canThrow() && snowballCount < 3) {
            player1->throwSnowball();
            Snowball* sb = new Snowball();
            //sb->setTexture("./assets/projectiles/snowball.png");
            float dir = player1->getFacingRight() ? 1.0f : -1.0f;
            sb->launch(player1->getX() + 16, player1->getY() + 20, dir, 1, 600.0f);
            addSnowball(sb);
        }
    }

    // Player 2
    if (isPlayerAlive(player2)) {
        bool throwKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0);
        if (throwKey && player2->canThrow() && snowballCount < 3) {
            player2->throwSnowball();
            Snowball* sb = new Snowball();
            //sb->setTexture("./assets/projectiles/snowball.png");
            float dir = player2->getFacingRight() ? 1.0f : -1.0f;
            sb->launch(player2->getX() + 16, player2->getY() + 20, dir, 1, 600.0f);
            addSnowball(sb);
        }
    }
}

void GameState::checkSnowballEnemyCollisions() {
    for (int s = 0; s < snowballCount; s++) {
        if (!snowballs[s] || !snowballs[s]->isActive()) continue;
        HitBox* sbHit = snowballs[s]->getHitbox();

        for (int e = 0; e < enemyCount; e++) {
            if (!enemies[e] || enemies[e]->getIsEncased()) continue;
            HitBox* eHit = enemies[e]->getHitbox();
            if (!sbHit || !eHit) continue;

            if (sbHit->intersects(*eHit)) {
                // Hit! Apply snowball power
                enemies[e]->hitBySnowball(snowballs[s]->getPower());
                snowballs[s]->deactivate();

                // Boss special handling
                if (strcmp(enemies[e]->getEnemyName(), "Mogera") == 0 && activeMogera) {
                    activeMogera->hitBySnowballBoss(snowballs[s]->getPower());
                }
                if (strcmp(enemies[e]->getEnemyName(), "Gamakichi") == 0 && activeGamakichi) {
                    activeGamakichi->hitBySnowballBoss(snowballs[s]->getPower());
                }

                // Score for encasing
                if (enemies[e]->getIsEncased()) {
                    score += enemies[e]->getScoreValue();
                    gems += enemies[e]->getGemDrop();
                    // Chance to spawn power-up
                    if (rand() % 3 == 0) {
                        powerUpSystem.spawnPowerUp(enemies[e]->getX(), enemies[e]->getY());
                    }
                    // Star bonus spawn
                    if (starBonusLevel && !starSpawnedThisLevel && rand() % 4 == 0) {
                        powerUpSystem.spawnStarBonus(enemies[e]->getX(), enemies[e]->getY());
                        starSpawnedThisLevel = true;
                    }
                }
                break;
            }
        }
    }
}

void GameState::checkPlayerEnemyCollisions() {
    Player* players[2] = { player1, player2 };
    for (int p = 0; p < playerCount; p++) {
        if (!isPlayerAlive(players[p]) || players[p]->isDying) continue;
        HitBox* pHit = players[p]->getHitbox();

        for (int e = 0; e < enemyCount; e++) {
            if (!enemies[e]) continue;
            HitBox* eHit = enemies[e]->getHitbox();
            if (!pHit || !eHit) continue;

            if (pHit->intersects(*eHit)) {
                if (enemies[e]->getIsEncased() && !enemies[e]->getIsRolling()) {
                    // Kick encased enemy
                    float dir = players[p]->getFacingRight() ? 1.0f : -1.0f;
                    enemies[e]->startRoll(dir);
                }
                else if (!enemies[e]->getIsEncased()) {
                    // Take damage
                    players[p]->takeDamage();
                    hud.setLivesP1(player1->getLives());
                    if (player2) hud.setLivesP2(player2->getLives());

                    float sx = (p == 0) ? currentLevel->getPlayer1SpawnX() : currentLevel->getPlayer2SpawnX();
                    float sy = (p == 0) ? currentLevel->getPlayer1SpawnY() : currentLevel->getPlayer2SpawnY();
                    respawnPlayerIfAlive(players[p], sx, sy);

                    if (!isPlayerAlive(players[p])) break;
                    // Brief invulnerability: reposition
                    //players[p]->setPosition(currentLevel->getPlayer1SpawnX(),
                    //    currentLevel->getPlayer1SpawnY());

                }
            }
        }
    }
}

void GameState::checkRollingEnemyCollisions() {
    for (int r = 0; r < enemyCount; r++) {
        if (!enemies[r] || !enemies[r]->getIsRolling()) continue;
        HitBox* rHit = enemies[r]->getHitbox();

        for (int e = 0; e < enemyCount; e++) {
            if (r == e || !enemies[e] || enemies[e]->getIsEncased()) continue;
            HitBox* eHit = enemies[e]->getHitbox();
            if (!rHit || !eHit) continue;

            if (rHit->intersects(*eHit)) {
                // Chain kill!
                enemies[e]->hitBySnowball(2);
                chainKillCount++;
                score += (int)(enemies[e]->getScoreValue() * 1.1f);
                if (enemies[e]->getIsEncased()) {
                    gems += enemies[e]->getGemDrop();
                }
            }
        }
    }
}

void GameState::checkPlayerKnifeCollisions() {
    Player* players[2] = { player1, player2 };

    for (int e = 0; e < enemyCount; e++) {
        if (strcmp(enemies[e]->getEnemyName(), "Tornado") != 0) continue;

        Tornado* tornado = (Tornado*)enemies[e];
        if (!tornado) continue;

        Knife** knives = tornado->knives;
        int knifeCount = tornado->knifeCount;

        for (int k = 0; k < knifeCount; k++) {
            //std::cout << "Tornado found! Knives: " << tornado->knifeCount << std::endl;
            if (!knives[k] || !knives[k]->isActive()) continue;

            for (int p = 0; p < playerCount; p++) {
                if (!isPlayerAlive(players[p]) || players[p]->isDying) continue;
                HitBox* pHit = players[p]->getHitbox();
                HitBox* kHit = knives[k]->getHitbox();
                if (pHit && kHit && pHit->intersects(*kHit)) {
                    players[p]->takeDamage();
                    knives[k]->deactivate();

                    hud.setLivesP1(player1->getLives());
                    if (player2) hud.setLivesP2(player2->getLives());

                    float sx = (p == 0) ? currentLevel->getPlayer1SpawnX() : currentLevel->getPlayer2SpawnX();
                    float sy = (p == 0) ? currentLevel->getPlayer1SpawnY() : currentLevel->getPlayer2SpawnY();
                    respawnPlayerIfAlive(players[p], sx, sy);
                }
            }
        }
    }
}

void GameState::checkPlayerRocketCollisions() {
    Player* players[2] = { player1, player2 };

    if (activeGamakichi) {
        ArtilleryRocket** rockets = activeGamakichi->getRockets();
        int rocketCount = activeGamakichi->getRocketCount();

        for (int r = 0; r < rocketCount; r++) {
            if (!rockets[r] || !rockets[r]->isActive()) continue;

            for (int p = 0; p < playerCount; p++) {
                if (!isPlayerAlive(players[p]) || players[p]->isDying) continue;
                HitBox* pHit = players[p]->getHitbox();
                HitBox* rHit = rockets[r]->getHitbox();
                if (pHit && rHit && pHit->intersects(*rHit)) {
                    players[p]->takeDamage();
                    rockets[r]->explode();

                    hud.setLivesP1(player1->getLives());
                    if (player2) hud.setLivesP2(player2->getLives());

                    float sx = (p == 0) ? currentLevel->getPlayer1SpawnX() : currentLevel->getPlayer2SpawnX();
                    float sy = (p == 0) ? currentLevel->getPlayer1SpawnY() : currentLevel->getPlayer2SpawnY();
                    respawnPlayerIfAlive(players[p], sx, sy);
                }
            }
        }
    }
}

void GameState::checkBossChildrenSnowballCollisions() {
    // Mogera children vs snowballs
    if (activeMogera) {
        Botom** children = activeMogera->getChildren();
        int childCount = activeMogera->getChildCount();
        for (int c = 0; c < childCount; c++) {
            if (!children[c] || children[c]->getIsEncased()) continue;
            for (int s = 0; s < snowballCount; s++) {
                if (!snowballs[s] || !snowballs[s]->isActive()) continue;
                if (snowballs[s]->getHitbox()->intersects(*children[c]->getHitbox())) {
                    children[c]->hitBySnowball(snowballs[s]->getPower());
                    snowballs[s]->deactivate();
                    if (children[c]->getIsEncased()) {
                        score += children[c]->getScoreValue();
                    }
                    break;
                }
            }
        }
    }

    //// Gamakichi children vs snowballs
    //if (activeGamakichi) {
    //    Botom** children = activeGamakichi->getChildren();
    //    int childCount = activeGamakichi->getChildCount();
    //    for (int c = 0; c < childCount; c++) {
    //        if (!children[c] || children[c]->getIsEncased()) continue;
    //        for (int s = 0; s < snowballCount; s++) {
    //            if (!snowballs[s] || !snowballs[s]->isActive()) continue;
    //            if (snowballs[s]->getHitbox()->intersects(*children[c]->getHitbox())) {
    //                children[c]->hitBySnowball(snowballs[s]->getPower());
    //                snowballs[s]->deactivate();
    //                if (children[c]->getIsEncased()) {
    //                    score += children[c]->getScoreValue();
    //                }
    //                break;
    //            }
    //        }
    //    }
    //}
}

void GameState::checkAllCollisions() {
    checkSnowballEnemyCollisions();
    checkRollingEnemyCollisions();
    checkPlayerEnemyCollisions();
    checkPlayerKnifeCollisions();
    checkPlayerRocketCollisions();
    checkPlayerBossChildCollisions();
    checkBossChildrenSnowballCollisions();
}

void GameState::updateBossHealthBars() {
    if (activeMogera && !activeMogera->getIsEncased()) {
        hud.showBossHealth("Mogera", activeMogera->getCurrentBossHealth(),
            activeMogera->getMaxBossHealth());
    }
    else if (activeGamakichi && !activeGamakichi->getIsEncased()) {
        hud.showBossHealth("Gamakichi", activeGamakichi->getCurrentBossHealth(),
            activeGamakichi->getMaxBossHealth());
    }
    else {
        hud.hideBossHealth();
    }
}

void GameState::checkLevelComplete() {
    if (isLevelComplete || isGameOver) return;

    bool allDefeated = true;
    bool bossDying = false;

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i] && !enemies[i]->getIsEncased()) {
            if (strcmp(enemies[i]->getEnemyName(), "Mogera") == 0) {
                if (!((Mogera*)enemies[i])->isDefeated()) allDefeated = false;
                else bossDying = true;  // Defeated but still playing death anim
            }
            else if (strcmp(enemies[i]->getEnemyName(), "Gamakichi") == 0) {
                if (!((Gamakichi*)enemies[i])->isDefeated()) allDefeated = false;
                else bossDying = true;
            }
            else {
                allDefeated = false;
            }
            if (!allDefeated) break;
        }
    }

    // Wait for boss death animation
    if (bossDying) {
        bossDeathDelay += 0.016;
        if (bossDeathDelay < 2.0f) return;  // Wait 2 seconds
    }

    if (allDefeated && enemyCount > 0) {
        levelComplete();
    }
}

void GameState::levelComplete() {
    isLevelComplete = true;
    levelCompleteTimer = 2.5f;

    if (activeMogera && activeMogera->isDefeated()) {
        gems += 200;
    }
    if (activeGamakichi && activeGamakichi->isDefeated()) {
        gems += 500;
    }

    if (isBonusLevel) {
        spawnBonusRain();
    }

    // Save to auth
    if (sceneManager && sceneManager->gameManager && sceneManager->gameManager->authManager) {
        AuthManager* auth = sceneManager->gameManager->authManager;
        auth->updateGems(gems);
        auth->updateScore(score);
    }

    saveManager.saveProgress(currentLevelNumber, player1->getLives(), score, gems);
    if (sceneManager && sceneManager->gameManager && sceneManager->gameManager->authManager) {
        AuthManager* auth = sceneManager->gameManager->authManager;
        auth->updateGems(gems);
        auth->updateScore(score);
        auth->updateLevel(currentLevelNumber + 1);  // Save current level
    }
    if (!isBonusLevel) {
        hud.setGems(gems);
        hud.setScore(score);
        hud.showLevelComplete(score, gems);
    }

}
void GameState::setStartLevel(int level) {
    startLevel = level;
}
void GameState::spawnBonusRain() {
    bonusItemsCollected = 0;
    for (int i = 0; i < bonusItemsMax; i++) {
        bonusItemX[i] = (float)(rand() % 1200) + 40.0f;
        bonusItemY[i] = -20.0f - (rand() % 100);
        bonusItemActive[i] = true;
    }
    bonusRainTimer = 5.0f;
}
void GameState::setCharacterSelection(int p1, int p2) {
    p1CharIndex = p1;
    p2CharIndex = p2;
}
void GameState::updateBonusRain(float dt) {
    if (!isBonusLevel || !isLevelComplete) return;
    bonusRainTimer -= dt;

    for (int i = 0; i < bonusItemsMax; i++) {
        if (!bonusItemActive[i]) continue;
        bonusItemY[i] += 200.0f * dt;
        if (bonusItemY[i] > 740) {
            bonusItemActive[i] = false;
        }

        // Check player collection
        Player* players[2] = { player1, player2 };
        for (int p = 0; p < playerCount; p++) {
            if (!players[p]) continue;
            float dx = players[p]->getX() + 16 - bonusItemX[i];
            float dy = players[p]->getY() + 24 - bonusItemY[i];
            if (dx * dx + dy * dy < 30 * 30 && bonusItemActive[i]) {
                bonusItemActive[i] = false;
                score += 1000;
                gems += 10;
                bonusItemsCollected++;
            }
        }
    }
}

void GameState::drawBonusRain(sf::RenderWindow& window) {
    if (!isBonusLevel || !isLevelComplete) return;
    for (int i = 0; i < bonusItemsMax; i++) {
        if (!bonusItemActive[i]) continue;
        if (gemTextureLoaded) {
            gemSprite.setPosition(bonusItemX[i] - 8, bonusItemY[i] - 8);
            gemSprite.setScale(0.8f, 0.8f);
            window.draw(gemSprite);
        }
        else {
            sf::CircleShape gem(8);
            gem.setPosition(bonusItemX[i] - 8, bonusItemY[i] - 8);
            gem.setFillColor(sf::Color::Cyan);
            window.draw(gem);
        }
    }
}
void GameState::setLevelLoader(LevelLoader* loader) {
    levelLoader = loader;
    totalLevels = loader->getTotalLevels();
}

void GameState::setPlayerMode(int mode) {
    playerCount = mode;
}
void GameState::loadNextLevel() {
    currentLevelNumber++;
    if (currentLevelNumber > totalLevels) {
        // Game complete!
        saveManager.saveLeaderboardEntry(
            sceneManager->gameManager->authManager->getUsername(),
            score, totalLevels);
        gameOver();
        return;
    }
    loadLevel(currentLevelNumber);
    isLevelComplete = false;
}

void GameState::gameOver() {
    isGameOver = true;
    saveManager.saveLeaderboardEntry(
        sceneManager->gameManager->authManager->getUsername(),
        score, currentLevelNumber);
    hud.showGameOver(score, currentLevelNumber);
}

void GameState::onEnter() {
    gemTextureLoaded = gemTexture.loadFromFile("./assets/hud/gem.png");
    if (gemTextureLoaded) gemSprite.setTexture(gemTexture);
    // 1. Create players FIRST
    const char* charPaths[4] = {
    "./assets/characters/nick_spritesheet.png",
    "./assets/characters/tom_spritesheet.png",
    "./assets/characters/frost_spritesheet.png",
    "./assets/characters/chill_spritesheet.png"
    };
    if (!player1) {
        player1 = new Player();
        player1->init(1, 100, 500);
        player1->setKeys(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::Space);
        player1->setJumpSound("./assets/audio/jump.wav");
        player1->setThrowSound("./assets/audio/snowball.wav");
        // if (player1Texture.loadFromFile("./assets/player/player1.png")) {
           //  player1->setTexture("./assets/player/player1.png");
        // }
        Spritesheet* playerSprite = nullptr;
        switch (p1CharIndex) {
        case 0:
            playerSprite = new Spritesheet(6, "./assets/characters/nick_spritesheet.png", { 139,116 });
            // Anim 0: Idle — row 0, frame 1
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(142, 117), 1);

            // Anim 1: Run — row 0, frames 2, 3, 4
            playerSprite->createAnim(1, 3, sf::Vector2i(142, 0), sf::Vector2i(142, 117), 10);

            // Anim 2: Jump — row 1, frame 1
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 117), sf::Vector2i(142, 117), 1);

            // Anim 3: Throw — row 1, frames 2, 3, 4
            playerSprite->createAnim(3, 3, sf::Vector2i(142, 117), sf::Vector2i(142, 117), 12);

            // Anim 4: Fall — row 2, frame 2
            playerSprite->createAnim(4, 1, sf::Vector2i(142, 234), sf::Vector2i(142, 117), 1);

            // Anim 5: Death — row 2, frames 3, 4
            playerSprite->createAnim(5, 2, sf::Vector2i(284, 234), sf::Vector2i(142, 117), 4);
            playerSprite->setSpriteOrigin(71, 58);
            break;
        case 1:
            playerSprite = new Spritesheet(6, "./assets/characters/tom_spritesheet.png", { 139,116 });
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(152, 117), 1);
            playerSprite->createAnim(1, 3, sf::Vector2i(152, 0), sf::Vector2i(152, 117), 10);
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 117), sf::Vector2i(152, 117), 1);
            playerSprite->createAnim(3, 3, sf::Vector2i(152, 117), sf::Vector2i(152, 117), 12);
            playerSprite->createAnim(4, 1, sf::Vector2i(152, 234), sf::Vector2i(152, 117), 1);
            playerSprite->createAnim(5, 2, sf::Vector2i(304, 234), sf::Vector2i(152, 117), 4);
            playerSprite->setSpriteOrigin(76, 58);
            break;
        case 2:
            playerSprite = new Spritesheet(6, "./assets/characters/frost_spritesheet.png", { 139,116 });
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(139, 116), 1);
            playerSprite->createAnim(1, 3, sf::Vector2i(139, 0), sf::Vector2i(139, 116), 10);
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 116), sf::Vector2i(139, 116), 1);
            playerSprite->createAnim(3, 3, sf::Vector2i(139, 116), sf::Vector2i(139, 116), 12);
            playerSprite->createAnim(4, 1, sf::Vector2i(139, 232), sf::Vector2i(139, 116), 1);
            playerSprite->createAnim(5, 2, sf::Vector2i(278, 232), sf::Vector2i(139, 116), 4);
            playerSprite->setSpriteOrigin(69, 58);
            break;
        case 3:
            playerSprite = new Spritesheet(6, "./assets/characters/chilli_spritesheet.png", { 139,116 });
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(120, 116), 1);
            playerSprite->createAnim(1, 3, sf::Vector2i(120, 0), sf::Vector2i(120, 116), 10);
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 116), sf::Vector2i(120, 116), 1);
            playerSprite->createAnim(3, 3, sf::Vector2i(120, 116), sf::Vector2i(120, 116), 12);
            playerSprite->createAnim(4, 1, sf::Vector2i(120, 232), sf::Vector2i(120, 116), 1);
            playerSprite->createAnim(5, 2, sf::Vector2i(240, 232), sf::Vector2i(120, 116), 4);
            playerSprite->setSpriteOrigin(60, 58);
            break;
        }
        playerSprite->setAnimLoop(0, true);
        playerSprite->setAnimLoop(1, true);
        playerSprite->setAnimLoop(2, false);
        playerSprite->setAnimLoop(3, false);
        playerSprite->setAnimLoop(4, false);
        playerSprite->setAnimLoop(5, false);

        player1->setSpritesheet(playerSprite);

    }
    if (playerCount == 2 && !player2) {
        player2 = new Player();
        player2->init(2, 200, 500);
        player2->setKeys(sf::Keyboard::Numpad4, sf::Keyboard::Numpad6,
            sf::Keyboard::Numpad8, sf::Keyboard::Numpad0);
        player2->setJumpSound("./assets/audio/jump.wav");
        player2->setThrowSound("./assets/audio/snowball.wav");
        // if (player2Texture.loadFromFile("./assets/player/player1.png")) {
             //player2->setTexture("./assets/player/player1.png");
        // }
         //player2->setSpritesheet(charPaths[p2CharIndex], 32, 48, 4);
        Spritesheet* playerSprite = nullptr;
        switch (p2CharIndex) {
        case 0:
            playerSprite = new Spritesheet(6, "./assets/characters/nick_spritesheet.png", { 139,116 });
            // Anim 0: Idle — row 0, frame 1
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(142, 117), 1);

            // Anim 1: Run — row 0, frames 2, 3, 4
            playerSprite->createAnim(1, 3, sf::Vector2i(142, 0), sf::Vector2i(142, 117), 10);

            // Anim 2: Jump — row 1, frame 1
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 117), sf::Vector2i(142, 117), 1);

            // Anim 3: Throw — row 1, frames 2, 3, 4
            playerSprite->createAnim(3, 3, sf::Vector2i(142, 117), sf::Vector2i(142, 117), 12);

            // Anim 4: Fall — row 2, frame 2
            playerSprite->createAnim(4, 1, sf::Vector2i(142, 234), sf::Vector2i(142, 117), 1);

            // Anim 5: Death — row 2, frames 3, 4
            playerSprite->createAnim(5, 2, sf::Vector2i(284, 234), sf::Vector2i(142, 117), 4);
            playerSprite->setSpriteOrigin(71, 58);
            break;
        case 1:
            playerSprite = new Spritesheet(6, "./assets/characters/tom_spritesheet.png", { 139,116 });
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(152, 117), 1);
            playerSprite->createAnim(1, 3, sf::Vector2i(152, 0), sf::Vector2i(152, 117), 10);
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 117), sf::Vector2i(152, 117), 1);
            playerSprite->createAnim(3, 3, sf::Vector2i(152, 117), sf::Vector2i(152, 117), 12);
            playerSprite->createAnim(4, 1, sf::Vector2i(152, 234), sf::Vector2i(152, 117), 1);
            playerSprite->createAnim(5, 2, sf::Vector2i(304, 234), sf::Vector2i(152, 117), 4);
            playerSprite->setSpriteOrigin(76, 58);
            break;
        case 2:
            playerSprite = new Spritesheet(6, "./assets/characters/frost_spritesheet.png", { 139,116 });
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(139, 116), 1);
            playerSprite->createAnim(1, 3, sf::Vector2i(139, 0), sf::Vector2i(139, 116), 10);
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 116), sf::Vector2i(139, 116), 1);
            playerSprite->createAnim(3, 3, sf::Vector2i(139, 116), sf::Vector2i(139, 116), 12);
            playerSprite->createAnim(4, 1, sf::Vector2i(139, 232), sf::Vector2i(139, 116), 1);
            playerSprite->createAnim(5, 2, sf::Vector2i(278, 232), sf::Vector2i(139, 116), 4);
            playerSprite->setSpriteOrigin(69, 58);
            break;
        case 3:
            playerSprite = new Spritesheet(6, "./assets/characters/chilli_spritesheet.png", { 139,116 });
            playerSprite->createAnim(0, 1, sf::Vector2i(0, 0), sf::Vector2i(120, 116), 1);
            playerSprite->createAnim(1, 3, sf::Vector2i(120, 0), sf::Vector2i(120, 116), 10);
            playerSprite->createAnim(2, 1, sf::Vector2i(0, 116), sf::Vector2i(120, 116), 1);
            playerSprite->createAnim(3, 3, sf::Vector2i(120, 116), sf::Vector2i(120, 116), 12);
            playerSprite->createAnim(4, 1, sf::Vector2i(120, 232), sf::Vector2i(120, 116), 1);
            playerSprite->createAnim(5, 2, sf::Vector2i(240, 232), sf::Vector2i(120, 116), 4);
            playerSprite->setSpriteOrigin(60, 58);
            break;
        }
        playerSprite->setAnimLoop(0, true);
        playerSprite->setAnimLoop(1, true);
        playerSprite->setAnimLoop(2, false);
        playerSprite->setAnimLoop(3, false);
        playerSprite->setAnimLoop(4, false);
        playerSprite->setAnimLoop(5, false);

        player2->setSpritesheet(playerSprite);
    }

    // 2. NOW load auth (players exist)
    if (sceneManager && sceneManager->gameManager) {
        AuthManager* auth = sceneManager->gameManager->authManager;
        gems = auth->getGems();
        player1->setLives(2);
        if (player2) player2->setLives(2);

        storedExtraLives = auth->getSecondChanceChips();
        storedSpeedBoosts = auth->getSpeedBoosts();
        storedSnowballPowers = auth->getBalloons();
        storedRangeBoosts = auth->getDistancePUP();

        shopSystem.setAuthManager(auth);
        shopSystem.setPlayer(player1);
        saveManager.setAuthManager(auth);
    }

    // 3. Init HUD and start
    hud.init();

    hud.setGems(gems);
    if (playerCount == 2) {
        hud.setTwoPlayer(true);
    }
    hud.setLivesP1(player1->getLives());

    if (player2) hud.setLivesP2(player2->getLives());
    loadGlobalTiles();
    loadLevel(startLevel);
}

void GameState::onExit() {
    if (sceneManager && sceneManager->gameManager) {
        sceneManager->gameManager->authManager->updateGems(gems);
        sceneManager->gameManager->authManager->updateScore(score);
    }
}

void GameState::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    handleGameInput(event);
}

//void GameState::update(float dt) {
//    //if (isPaused || isGameOver) return;
//    // Game over when both dead
//    bool p1Dead = player1 && player1->getLives() <= 0 && !player1->isDying;
//    bool p2Dead = !player2 || (player2->getLives() <= 0 && !player2->isDying);
//
//    if (p1Dead && p2Dead) {
//        gameOver();
//        return;
//    }
//    if (isLevelComplete) {
//        levelCompleteTimer -= dt;
//        updateBonusRain(dt);
//        if (levelCompleteTimer <= 0 && bonusRainTimer <= 0) {
//            hud.setGems(gems);
//            hud.setScore(score);
//            hud.showLevelComplete(score, gems);
//            loadNextLevel();
//        }
//        hud.update(dt);
//        return;
//    }
//
//    if (!currentLevel) return;
//
//
//
//    int platCount = currentLevel->getPlatformCount();
//    Platform** platforms = new Platform * [platCount];
//    for (int i = 0; i < platCount; i++) {
//        platforms[i] = currentLevel->getPlatform(i);
//    }
//
//    // Update players
//    player1->update(dt);
//    player1->checkPlatformCollisions(platforms, platCount);
//    if (player2) {
//        player2->update(dt);
//        player2->checkPlatformCollisions(platforms, platCount);
//    }
//
//    // Process throws
//    processPlayerThrows();
//
//    // Update enemies
//    for (int i = 0; i < enemyCount; i++) {
//        if (enemies[i]) {
//            if (strcmp(enemies[i]->getEnemyName(), "Mogera") == 0 ||
//                strcmp(enemies[i]->getEnemyName(), "Gamakichi") == 0) {
//                enemies[i]->updateAI(dt, platforms, platCount, isPlayerAlive(player1) ? player1 : nullptr, isPlayerAlive(player2) ? player2 : nullptr);
//                
//            }
//            else {
//                enemies[i]->updateCommon(dt, platforms, platCount, isPlayerAlive(player1) ? player1 : nullptr, isPlayerAlive(player2) ? player2 : nullptr);
//            }
//        }
//    }
//
//    // Update snowballs
//    for (int i = 0; i < snowballCount; i++) {
//        if (snowballs[i]) snowballs[i]->update(dt);
//    }
//
//    // Cleanup inactive snowballs
//    for (int i = snowballCount - 1; i >= 0; i--) {
//        if (snowballs[i] && !snowballs[i]->isActive()) {
//            removeSnowball(i);
//        }
//    }
//
//    // Cleanup rolling enemies off screen
//    for (int i = enemyCount - 1; i >= 0; i--) {
//        if (enemies[i] && enemies[i]->getIsRolling()) {
//            float ey = enemies[i]->getY();
//            float ex = enemies[i]->getX();
//            if (ey > 800 || ex < -100 || ex > 1380) {
//                removeEnemy(i);
//            }
//        }
//    }
//
//    // Update systems
//    powerUpSystem.update(dt);
//    powerUpSystem.checkCollection(isPlayerAlive(player1) ? player1 : nullptr, isPlayerAlive(player2) ? player2 : nullptr, &hud);
//    if (isBonusLevel && isLevelComplete) updateBonusRain(dt);
//
//    delete[] platforms;
//
//    // Collisions
//    checkAllCollisions();
//
//    // UI
//    updateBossHealthBars();
//    hud.setScore(score);
//    hud.setGems(gems);
//    hud.setLives(player1->getLives());
//    hud.update(dt);
//
//    checkLevelComplete();
//}
void GameState::update(float dt) {
    if (isPaused || isGameOver) return;

    bool p1Out = !isPlayerAlive(player1);
    bool p2Out = !player2 || !isPlayerAlive(player2);
    if (p1Out && p2Out) {
        gameOver();
        return;
    }

    if (!currentLevel) return;

    int platCount = currentLevel->getPlatformCount();
    Platform** platforms = new Platform * [platCount];
    for (int i = 0; i < platCount; i++) {
        platforms[i] = currentLevel->getPlatform(i);
    }

    // Update only players who still have lives. A player with 0 lives stays out while the other continues.
    if (isPlayerAlive(player1)) {
        player1->update(dt);
        player1->checkPlatformCollisions(platforms, platCount);
    }
    if (isPlayerAlive(player2)) {
        player2->update(dt);
        player2->checkPlatformCollisions(platforms, platCount);
    }

    if (isLevelComplete) {
        levelCompleteTimer -= dt;
        updateBonusRain(dt);
        if (levelCompleteTimer <= 0 && bonusRainTimer <= 0) {
            loadNextLevel();
        }
        hud.update(dt);
        delete[] platforms;
        return;
    }

    // Rest only when not level complete
    processPlayerThrows();

    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i]) {
            if (strcmp(enemies[i]->getEnemyName(), "Mogera") == 0 ||
                strcmp(enemies[i]->getEnemyName(), "Gamakichi") == 0) {
                enemies[i]->updateAI(dt, platforms, platCount, isPlayerAlive(player1) ? player1 : nullptr, isPlayerAlive(player2) ? player2 : nullptr);
            }
            else {
                enemies[i]->updateCommon(dt, platforms, platCount, isPlayerAlive(player1) ? player1 : nullptr, isPlayerAlive(player2) ? player2 : nullptr);
            }
        }
    }

    for (int i = 0; i < snowballCount; i++) {
        if (snowballs[i]) snowballs[i]->update(dt);
    }

    for (int i = snowballCount - 1; i >= 0; i--) {
        if (snowballs[i] && !snowballs[i]->isActive()) removeSnowball(i);
    }

    for (int i = enemyCount - 1; i >= 0; i--) {
        if (enemies[i] && enemies[i]->getIsRolling()) {
            float ey = enemies[i]->getY();
            float ex = enemies[i]->getX();
            if (ey > 800 || ex < -100 || ex > 1380) removeEnemy(i);
        }
    }

    powerUpSystem.update(dt);
    powerUpSystem.checkCollection(isPlayerAlive(player1) ? player1 : nullptr, isPlayerAlive(player2) ? player2 : nullptr, &hud);

    delete[] platforms;

    checkAllCollisions();
    updateBossHealthBars();
    hud.setScore(score);
    hud.setGems(gems);
    hud.setLivesP1(player1->getLives());
    if (player2) hud.setLivesP2(player2->getLives());
    hud.setInventory(storedSpeedBoosts, storedSnowballPowers, storedRangeBoosts, storedBalloons, storedExtraLives);
    hud.update(dt);
    checkLevelComplete();
}


void GameState::draw(sf::RenderWindow& window) {
    window.draw(bgSprite);

    if (!currentLevel) return;

    // Draw platforms with sprites
    for (int i = 0; i < currentLevel->getPlatformCount(); i++) {
        Platform* p = currentLevel->getPlatform(i);
        Tile* t = p->getHeadTile();
        while (t) {
            int type = t->getTileType();
            if (type >= 0 && type < MAX_GLOBAL_TILES && globalTileTex[type].getSize().x > 0) {
                globalTileSpr[type].setPosition(t->getX(), t->getY());
                window.draw(globalTileSpr[type]);
            }
            else {
                sf::RectangleShape tileRect(sf::Vector2f(t->getWidth(), t->getHeight()));
                tileRect.setPosition(t->getX(), t->getY());
                switch (type) {
                case 0: tileRect.setFillColor(sf::Color(120, 80, 40)); break;
                case 1: tileRect.setFillColor(sf::Color(90, 90, 90)); break;
                case 2: tileRect.setFillColor(sf::Color(150, 130, 90)); break;
                }
                tileRect.setOutlineColor(sf::Color(60, 40, 20));
                tileRect.setOutlineThickness(1);
                window.draw(tileRect);
            }
            t = t->getNext();
        }
    }

    // Draw enemies
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i]) enemies[i]->draw(window);
    }

    // Draw snowballs
    for (int i = 0; i < snowballCount; i++) {
        if (snowballs[i]) snowballs[i]->draw(window);
    }

    // Draw players
    if (isPlayerAlive(player1)) player1->draw(window);
    if (isPlayerAlive(player2)) player2->draw(window);

    // Draw power-ups
    powerUpSystem.draw(window);

    // Bonus rain
    drawBonusRain(window);

    // Hitbox debug
    if (showHitboxes) {
        for (int i = 0; i < currentLevel->getPlatformCount(); i++) {
            Platform* p = currentLevel->getPlatform(i);
            Tile* t = p->getHeadTile();
            while (t) {
                sf::RectangleShape dbg(sf::Vector2f(t->getWidth(), t->getHeight()));
                dbg.setPosition(t->getX(), t->getY());
                dbg.setFillColor(sf::Color::Transparent);
                dbg.setOutlineColor(sf::Color::Blue);
                dbg.setOutlineThickness(2);
                window.draw(dbg);
                t = t->getNext();
            }
        }
        if (isPlayerAlive(player1)) player1->drawDebug(window);
        if (isPlayerAlive(player2)) player2->drawDebug(window);
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i]) enemies[i]->drawDebug(window);
        }
        for (int i = 0; i < snowballCount; i++) {
            if (snowballs[i]) snowballs[i]->drawDebug(window);
        }
    }

    // HUD
    hud.draw(window);
    hud.drawBossHealth(window);
    hud.drawOverlay(window);
    if (isPaused) {
        window.draw(pauseSprite);
    }
}

void GameState::onButtonClick(const std::string& buttonId) {}