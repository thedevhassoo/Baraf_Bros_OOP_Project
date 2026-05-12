#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "TextureButton.h"
#include "LevelData.h"

class LevelEditor : public Scene {
private:
    LevelData* levelData;
    int currentLevelNum;

    sf::Sprite ghostTile;
    int ghostTileType;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    sf::Texture tileTex[18];
    sf::Sprite tileSpr[18];
    int totalTileTypes;
    bool tilesLoaded;

    int currentTool;
    int selectedEnemyType;
    int selectedVariant;
    int tileStartIndex;

    Platform* currentPlatform;

    const char* enemyTypes[5];
    const char* variantNames[3];

    sf::Font font;
    sf::Text toolText;
    sf::Text levelText;
    sf::Text enemyInfoText;

    static const int BUTTON_COUNT = 17;
    TextureButton** allButtons;

    TextureButton* tileBtns[3];
    TextureButton* tilePrevBtn;
    TextureButton* tileNextBtn;
    TextureButton* enemyBtn;
    TextureButton* player1Btn;
    TextureButton* player2Btn;
    TextureButton* eraseBtn;
    TextureButton* saveBtn;
    TextureButton* loadBtn;
    TextureButton* clearBtn;
    TextureButton* backBtn;
    TextureButton* nextEnemyBtn;
    TextureButton* nextVariantBtn;
    TextureButton* levelUpBtn;
    TextureButton* levelDownBtn;

    sf::CircleShape p1Indicator;
    sf::CircleShape p2Indicator;

    void updateIndicators();
    void updateToolText();
    void snapToGrid(float& x, float& y);
    void placeTile(float x, float y);
    void placeEnemy(float x, float y);
    void loadBackground();
    void loadAllTiles();
    void updateTileButtonTextures();
    bool isClickOnButton(float mx, float my);

public:
    LevelEditor();
    ~LevelEditor();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
    void onEnter() override;
    void onExit() override;
};