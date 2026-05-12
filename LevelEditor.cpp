#define _CRT_SECURE_NO_WARNINGS
#include "LevelEditor.h"
#include "SceneManager.h"
#include "Platform.h"
#include "Tile.h"
#include "EnemyFactory.h"
#include <fstream>
#include <iostream>
#include "MenuScene.h"
LevelEditor::LevelEditor()
    : currentLevelNum(1), totalTileTypes(18), tilesLoaded(false),
    currentTool(0), selectedEnemyType(0), selectedVariant(0),
    tileStartIndex(0), currentPlatform(nullptr) {

    levelData = new LevelData();
    levelData->setLevelNumber(1);

    enemyTypes[0] = "Botom";
    enemyTypes[1] = "FlyingFoogaFoog";
    enemyTypes[2] = "Tornado";
    enemyTypes[3] = "Mogera";
    enemyTypes[4] = "Gamakichi";

    variantNames[0] = "Blue";
    variantNames[1] = "White";
    variantNames[2] = "Black";

    font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");

    allButtons = new TextureButton * [BUTTON_COUNT];
    for (int i = 0; i < BUTTON_COUNT; i++) allButtons[i] = nullptr;

    int idx = 0;

    // Row 1
    saveBtn = new TextureButton("./assets/editor/save.png", this, "save");
    allButtons[idx++] = saveBtn;

    loadBtn = new TextureButton("./assets/editor/load.png", this, "load");
    allButtons[idx++] = loadBtn;

    clearBtn = new TextureButton("./assets/editor/clear.png", this, "clear");
    allButtons[idx++] = clearBtn;

    eraseBtn = new TextureButton("./assets/editor/erase.png", this, "erase");
    allButtons[idx++] = eraseBtn;

    backBtn = new TextureButton("./assets/editor/back.png", this, "back");
    allButtons[idx++] = backBtn;

    levelDownBtn = new TextureButton("./assets/editor/down.png", this, "levelDown");
    allButtons[idx++] = levelDownBtn;

    levelUpBtn = new TextureButton("./assets/editor/up.png", this, "levelUp");
    allButtons[idx++] = levelUpBtn;

    tilePrevBtn = new TextureButton("./assets/editor/left.png", this, "tilePrev");
    allButtons[idx++] = tilePrevBtn;

    tileBtns[0] = new TextureButton("./assets/tiles/tile0.png", this, "tile0");
    allButtons[idx++] = tileBtns[0];

    tileBtns[1] = new TextureButton("./assets/tiles/tile1.png", this, "tile1");
    allButtons[idx++] = tileBtns[1];

    tileBtns[2] = new TextureButton("./assets/tiles/tile2.png", this, "tile2");
    allButtons[idx++] = tileBtns[2];

    tileNextBtn = new TextureButton("./assets/editor/right.png", this, "tileNext");
    allButtons[idx++] = tileNextBtn;

    enemyBtn = new TextureButton("./assets/editor/enemy.png", this, "enemy");
    allButtons[idx++] = enemyBtn;

    nextEnemyBtn = new TextureButton("./assets/editor/next.png", this, "nextEnemy");
    allButtons[idx++] = nextEnemyBtn;

    nextVariantBtn = new TextureButton("./assets/editor/variant.png", this, "nextVariant");
    allButtons[idx++] = nextVariantBtn;

    // Row 2
    player1Btn = new TextureButton("./assets/editor/p1.png", this, "player1");
    allButtons[idx++] = player1Btn;

    player2Btn = new TextureButton("./assets/editor/p2.png", this, "player2");
    allButtons[idx++] = player2Btn;

    // Positions
    saveBtn->setPos({ 23, 13 });
    loadBtn->setPos({ 78, 19 });
    clearBtn->setPos({ 151, 21 });
    eraseBtn->setPos({ 223, 21 });
    backBtn->setPos({ 300, 21 });
    levelDownBtn->setPos({ 380, 21 });
    levelUpBtn->setPos({ 440, 21 });
    tilePrevBtn->setPos({ 572, 23 });
    tileBtns[0]->setPos({ 624, 4 });
    tileBtns[1]->setPos({ 731, 4 });
    tileBtns[2]->setPos({ 828, 4 });
    tileNextBtn->setPos({ 921, 23 });
    enemyBtn->setPos({ 1129, 9 });
    nextEnemyBtn->setPos({ 1239, 78 });
    nextVariantBtn->setPos({ 1231, 11 });
    player1Btn->setPos({ 0, 68 });
    player2Btn->setPos({ 107, 68 });

    p1Indicator.setRadius(10);
    p1Indicator.setFillColor(sf::Color(0, 200, 255, 150));
    p1Indicator.setOutlineColor(sf::Color::Cyan);
    p1Indicator.setOutlineThickness(2);

    p2Indicator.setRadius(10);
    p2Indicator.setFillColor(sf::Color(255, 100, 100, 150));
    p2Indicator.setOutlineColor(sf::Color::Red);
    p2Indicator.setOutlineThickness(2);

    toolText.setFont(font);
    toolText.setCharacterSize(16);
    toolText.setFillColor(sf::Color::Yellow);
	toolText.setOutlineColor(sf::Color::Black);
	toolText.setOutlineThickness(1);
    toolText.setStyle(sf::Text::Bold);
    toolText.setPosition(10, 170);

    levelText.setFont(font);
    levelText.setCharacterSize(22);
    levelText.setFillColor(sf::Color::Yellow);
	levelText.setOutlineColor(sf::Color::Black);
	levelText.setOutlineThickness(1);
    levelText.setStyle(sf::Text::Bold);
    levelText.setPosition(950, 20);

    enemyInfoText.setFont(font);
    enemyInfoText.setCharacterSize(14);
    enemyInfoText.setFillColor(sf::Color::Yellow);
	enemyInfoText.setOutlineColor(sf::Color::Black);
    enemyInfoText.setOutlineThickness(1);
    enemyInfoText.setStyle(sf::Text::Bold);
    enemyInfoText.setPosition(10, 190);

    updateToolText();
    updateIndicators();
}

LevelEditor::~LevelEditor() {
    delete levelData;
    if (currentPlatform) delete currentPlatform;
    for (int i = 0; i < BUTTON_COUNT; i++) delete allButtons[i];
    delete[] allButtons;
}

void LevelEditor::loadAllTiles() {
    if (tilesLoaded) return;
    for (int i = 0; i < 18; i++) {
        std::string path = "./assets/tiles/tile" + std::to_string(i) + ".png";
        if (tileTex[i].loadFromFile(path)) {
            tileSpr[i].setTexture(tileTex[i]);
        }
    }
    tilesLoaded = true;
}

void LevelEditor::updateTileButtonTextures() {
    int baseIndex = 8;  // tileBtns start at allButtons[8]

    for (int i = 0; i < 3; i++) {
        int tileIdx = tileStartIndex + i;
        if (tileIdx < 18 && tileTex[tileIdx].getSize().x > 0) {
            std::string path = "./assets/tiles/tile" + std::to_string(tileIdx) + ".png";
            std::string btnId = "tile" + std::to_string(i);

            delete allButtons[baseIndex + i];
            tileBtns[i] = new TextureButton(path, this, btnId);
            tileBtns[i]->setPos({ 624.0f + i * 107.0f, 4.0f });
            allButtons[baseIndex + i] = tileBtns[i];
        }
    }
}

void LevelEditor::loadBackground() {
    std::string path = "./assets/levels/level" + std::to_string(currentLevelNum) + "/bg_level" + std::to_string(currentLevelNum) + ".png";
    if (!bgTexture.loadFromFile(path)) {
        bgTexture.loadFromFile("./assets/levels/bg_default.png");
    }
    bgSprite.setTexture(bgTexture);
}

void LevelEditor::snapToGrid(float& x, float& y) {
    x = ((int)(x / 64)) * 64.0f;
    y = ((int)(y / 64)) * 64.0f;
}

void LevelEditor::placeTile(float x, float y) {
    snapToGrid(x, y);
    if (!currentPlatform) {
        currentPlatform = new Platform();
    }
    currentPlatform->addTile(currentTool, x, y);
}

void LevelEditor::placeEnemy(float x, float y) {
    levelData->addEnemySpawn(enemyTypes[selectedEnemyType], x, y, selectedVariant);
}

void LevelEditor::updateIndicators() {
    p1Indicator.setPosition(levelData->getPlayer1SpawnX() - 10,
        levelData->getPlayer1SpawnY() - 10);
    p2Indicator.setPosition(levelData->getPlayer2SpawnX() - 10,
        levelData->getPlayer2SpawnY() - 10);
}

void LevelEditor::updateToolText() {
    if (currentTool <= 17) {
        toolText.setString("Tool: Tile " + std::to_string(currentTool));
    }
    else if (currentTool == 18) {
        toolText.setString(std::string("Tool: Enemy [") + enemyTypes[selectedEnemyType] + "] [" + variantNames[selectedVariant] + "]");
    }
    else if (currentTool == 19) {
        toolText.setString("Tool: Player 1 Spawn");
    }
    else if (currentTool == 20) {
        toolText.setString("Tool: Player 2 Spawn");
    }
    else if (currentTool == 21) {
        toolText.setString("Tool: Erase (Right click removes last)");
    }

    enemyInfoText.setString(std::string("Enemy: ") + enemyTypes[selectedEnemyType] + " | Variant: " + variantNames[selectedVariant] + " | Boss: " + (EnemyFactory::isBoss(enemyTypes[selectedEnemyType]) ? "YES" : "No"));
}

bool LevelEditor::isClickOnButton(float mx, float my) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        sf::Vector2f pos = allButtons[i]->getPosition();
        float bw = 64;  // default, adjust to actual button size if TextureButton has getWidth
        float bh = 64;
        if (mx >= pos.x && mx <= pos.x + bw && my >= pos.y && my <= pos.y + bh) {
            return true;
        }
    }
    return false;
}

void LevelEditor::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        allButtons[i]->handleEvent(event, window);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float mx = (float)mousePos.x;
        float my = (float)mousePos.y;

        if (isClickOnButton(mx, my)) return;

        if (currentTool <= 17) {
            placeTile(mx, my);
        }
        else if (currentTool == 18) {
            placeEnemy(mx, my);
        }
        else if (currentTool == 19) {
            levelData->setPlayer1Spawn(mx, my);
            updateIndicators();
        }
        else if (currentTool == 20) {
            levelData->setPlayer2Spawn(mx, my);
            updateIndicators();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float mx = (float)mousePos.x;
        float my = (float)mousePos.y;

        if (isClickOnButton(mx, my)) return;

        if (currentPlatform && currentPlatform->getTileCount() > 0) {
            levelData->addPlatform(currentPlatform);
            currentPlatform = nullptr;
        }
        else if (currentTool == 21) {
            levelData->removeLastPlatform();
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
            int keyNum = event.key.code - sf::Keyboard::Num0;
            if (keyNum <= 5) currentTool = tileStartIndex + keyNum;
            else if (keyNum == 6) currentTool = 18;
            else if (keyNum == 7) currentTool = 19;
            else if (keyNum == 8) currentTool = 20;
            else if (keyNum == 9) currentTool = 21;
            updateToolText();
        }
        if (event.key.code == sf::Keyboard::Enter) {
            if (currentPlatform && currentPlatform->getTileCount() > 0) {
                levelData->addPlatform(currentPlatform);
                currentPlatform = nullptr;
            }
        }
        if (event.key.code == sf::Keyboard::V) {
            selectedVariant = (selectedVariant + 1) % 3;
            updateToolText();
        }
        if (event.key.code == sf::Keyboard::Escape) {
            if (currentPlatform && currentPlatform->getTileCount() > 0) {
                levelData->addPlatform(currentPlatform);
                currentPlatform = nullptr;
            }
            sceneManager->changeScene(new MenuScene());
            return;
        }
    }
}

void LevelEditor::update(float dt) {
    levelText.setString("Level: " + std::to_string(currentLevelNum));
}

void LevelEditor::draw(sf::RenderWindow& window) {
    window.draw(bgSprite);

    for (int i = 0; i < levelData->getPlatformCount(); i++) {
        Platform* p = levelData->getPlatform(i);
        Tile* t = p->getHeadTile();
        while (t) {
            int type = t->getTileType();
            if (type >= 0 && type < 18 && tileTex[type].getSize().x > 0) {
                tileSpr[type].setPosition(t->getX(), t->getY());
                window.draw(tileSpr[type]);
            }
            else {
                sf::RectangleShape rect(sf::Vector2f(64, 64));
                rect.setPosition(t->getX(), t->getY());
                rect.setFillColor(sf::Color(100, 70, 30));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                window.draw(rect);
            }
            t = t->getNext();
        }
    }

    if (currentPlatform) {
        Tile* t = currentPlatform->getHeadTile();
        while (t) {
            int type = t->getTileType();
            if (type >= 0 && type < 18 && tileTex[type].getSize().x > 0) {
                tileSpr[type].setPosition(t->getX(), t->getY());
                tileSpr[type].setColor(sf::Color(255, 255, 150, 150));
                window.draw(tileSpr[type]);
                tileSpr[type].setColor(sf::Color::White);
            }
            t = t->getNext();
        }
    }

    EnemySpawnPoint* e = levelData->getEnemyHead();
    while (e) {
        sf::Color dotColor = sf::Color::Red;
        if (e->getColorVariant() == 1) dotColor = sf::Color::Green;
        else if (e->getColorVariant() == 2) dotColor = sf::Color::Blue;

        sf::CircleShape dot(8);
        dot.setPosition(e->getX() - 8, e->getY() - 8);
        dot.setFillColor(dotColor);
        dot.setOutlineColor(sf::Color::White);
        dot.setOutlineThickness(1);

        if (EnemyFactory::isBoss(e->getEnemyType())) {
            dot.setRadius(14);
            dot.setPosition(e->getX() - 14, e->getY() - 14);
            dot.setOutlineColor(sf::Color::Yellow);
            dot.setOutlineThickness(2);
        }
        window.draw(dot);
        e = e->getNext();
    }
    // Ghost tile preview
    if (currentTool <= 17 && !isClickOnButton(
        (float)sf::Mouse::getPosition(window).x,
        (float)sf::Mouse::getPosition(window).y)) {

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float mx = (float)mousePos.x;
        float my = (float)mousePos.y;
        snapToGrid(mx, my);

        if (my >= 20) {  // Below toolbar
            if (currentTool >= 0 && currentTool < 18 && tileTex[currentTool].getSize().x > 0) {
                ghostTile.setTexture(tileTex[currentTool]);
                ghostTile.setPosition(mx, my);
                ghostTile.setColor(sf::Color(255, 255, 255, 150));  // Semi-transparent
                window.draw(ghostTile);
            }
        }
    }
    window.draw(p1Indicator);
    window.draw(p2Indicator);

    for (int i = 0; i < BUTTON_COUNT; i++) allButtons[i]->draw(window);

    window.draw(toolText);
    window.draw(levelText);
    window.draw(enemyInfoText);
}

void LevelEditor::onButtonClick(const std::string& buttonId) {
    if (buttonId == "tile0") currentTool = tileStartIndex + 0;
    else if (buttonId == "tile1") currentTool = tileStartIndex + 1;
    else if (buttonId == "tile2") currentTool = tileStartIndex + 2;
    else if (buttonId == "tilePrev") {
        if (tileStartIndex > 0) {
            tileStartIndex -= 3;
            updateTileButtonTextures();
        }
    }
    else if (buttonId == "tileNext") {
        if (tileStartIndex + 3 < 18) {
            tileStartIndex += 3;
            updateTileButtonTextures();
        }
    }
    else if (buttonId == "enemy") currentTool = 18;
    else if (buttonId == "player1") currentTool = 19;
    else if (buttonId == "player2") currentTool = 20;
    else if (buttonId == "erase") currentTool = 21;
    else if (buttonId == "nextEnemy") selectedEnemyType = (selectedEnemyType + 1) % 5;
    else if (buttonId == "nextVariant") selectedVariant = (selectedVariant + 1) % 3;
    else if (buttonId == "levelUp") {
        if (currentLevelNum < 20) {
            if (currentPlatform && currentPlatform->getTileCount() > 0) {
                levelData->addPlatform(currentPlatform);
                currentPlatform = nullptr;
            }
            currentLevelNum++;
            levelData->setLevelNumber(currentLevelNum);
            loadBackground();
        }
    }
    else if (buttonId == "levelDown") {
        if (currentLevelNum > 1) {
            if (currentPlatform && currentPlatform->getTileCount() > 0) {
                levelData->addPlatform(currentPlatform);
                currentPlatform = nullptr;
            }
            currentLevelNum--;
            levelData->setLevelNumber(currentLevelNum);
            loadBackground();
        }
    }
    else if (buttonId == "save") {
        if (currentPlatform && currentPlatform->getTileCount() > 0) {
            levelData->addPlatform(currentPlatform);
            currentPlatform = nullptr;
        }
        std::string path = "./assets/levels/level" + std::to_string(currentLevelNum) + "/level" + std::to_string(currentLevelNum) + ".txt";
        if (levelData->saveToFile(path.c_str())) {
            std::cout << "Level " << currentLevelNum << " saved!" << std::endl;
        }
    }
    else if (buttonId == "load") {
        if (currentPlatform && currentPlatform->getTileCount() > 0) {
            levelData->addPlatform(currentPlatform);
            currentPlatform = nullptr;
        }
        std::string path = "./assets/levels/level" + std::to_string(currentLevelNum) + "/level" + std::to_string(currentLevelNum) + ".txt";
        if (levelData->loadFromFile(path.c_str())) {
            currentLevelNum = levelData->getLevelNumber();
            loadBackground();
            updateIndicators();
            std::cout << "Level " << currentLevelNum << " loaded!" << std::endl;
        }
    }
    else if (buttonId == "clear") {
        levelData->clearPlatforms();
        levelData->clearEnemySpawns();
        levelData->setPlayer1Spawn(100, 500);
        levelData->setPlayer2Spawn(200, 500);
        updateIndicators();
    }
    else if (buttonId == "back") {
        if (currentPlatform && currentPlatform->getTileCount() > 0) {
            levelData->addPlatform(currentPlatform);
            currentPlatform = nullptr;
        }
        sceneManager->changeScene(nullptr);
    }
    updateToolText();
}

void LevelEditor::onEnter() {
    loadAllTiles();
    loadBackground();
}

void LevelEditor::onExit() {
    if (currentPlatform && currentPlatform->getTileCount() > 0) {
        levelData->addPlatform(currentPlatform);
        currentPlatform = nullptr;
    }
}