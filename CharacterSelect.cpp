#include "CharacterSelect.h"
#include "SceneManager.h"
#include "GameState.h"
#include "LevelLoader.h"
#include <cstring>

CharacterSelect::CharacterSelect()
    : p1Selection(0), p2Selection(0), isMultiplayer(false), confirmed(false),
    startBtn("./assets/editor/save.png", this, "start"),
    backBtn("./assets/editor/back.png", this, "back"),
    leftBtn("./assets/editor/left.png", this, "left"),
    rightBtn("./assets/editor/right.png", this, "right") {

    font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");

    // Dark overlay
    blockTexture.setSize(sf::Vector2f(1280, 720));
    blockTexture.setFillColor(sf::Color(0, 0, 0, 200));

    strcpy_s(charNames[0], "Nick");
    strcpy_s(charNames[1], "Tom");
    strcpy_s(charNames[2], "Frost");
    strcpy_s(charNames[3], "Chilli");

    const char* paths[4] = {
        "./assets/characters/nick.png",
        "./assets/characters/tom.png",
        "./assets/characters/frost.png",
        "./assets/characters/chilli.png"
    };

    float startX = 300.0f;
    float spacing = 180.0f;
    float charY = 250.0f;

    for (int i = 0; i < 4; i++) {
        if (charTextures[i].loadFromFile(paths[i])) {
            charSprites[i].setTexture(charTextures[i]);
            charSprites[i].setScale(2.0f, 2.0f);
            charSprites[i].setPosition(startX + spacing * i, charY);
        }

        charNameTexts[i].setFont(font);
        charNameTexts[i].setString(charNames[i]);
        charNameTexts[i].setCharacterSize(20);
        charNameTexts[i].setFillColor(sf::Color::White);
        charNameTexts[i].setPosition(startX + spacing * i + 35, charY + 110);
    }

    // Arrows
    arrowTexture.loadFromFile("./assets/editor/up.png");
    p1Arrow.setTexture(arrowTexture);
    p1Arrow.setColor(sf::Color::Cyan);
    p1Arrow.setScale(1.5f, 1.5f);
    p2Arrow.setTexture(arrowTexture);
    p2Arrow.setColor(sf::Color::Red);
    p2Arrow.setScale(1.5f, 1.5f);

    updateArrowPosition(1);
    updateArrowPosition(2);

    // Title
    titleText.setFont(font);
    titleText.setString("SELECT CHARACTER");
    titleText.setCharacterSize(34);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(420, 100);

    p1Label.setFont(font);
    p1Label.setString("P1: A/D to choose");
    p1Label.setCharacterSize(16);
    p1Label.setFillColor(sf::Color::Cyan);
    p1Label.setPosition(100, 180);

    p2Label.setFont(font);
    p2Label.setString("P2: Left/Right to choose");
    p2Label.setCharacterSize(16);
    p2Label.setFillColor(sf::Color::Red);
    p2Label.setPosition(100, 205);

    // Buttons
    startBtn.setPos({ 500, 550 });
    backBtn.setPos({ 650, 550 });
    leftBtn.setPos({ 200, 420 });
    rightBtn.setPos({ 980, 420 });
    continueMode = false;
    startLevel = 1;
}

CharacterSelect::~CharacterSelect() {}
void CharacterSelect::setContinueMode(bool mode) { continueMode = mode; }
void CharacterSelect::setStartLevel(int level) { startLevel = level; }
void CharacterSelect::setMultiplayer(bool mp) { isMultiplayer = mp; }
int CharacterSelect::getP1Selection() { return p1Selection; }
int CharacterSelect::getP2Selection() { return p2Selection; }
bool CharacterSelect::isConfirmed() { return confirmed; }

void CharacterSelect::updateArrowPosition(int player) {
    float startX = 300.0f;
    float spacing = 180.0f;
    int sel = (player == 1) ? p1Selection : p2Selection;
    sf::Sprite& arrow = (player == 1) ? p1Arrow : p2Arrow;

    arrow.setPosition(startX + spacing * sel + 20, 200.0f);
}

void CharacterSelect::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    startBtn.handleEvent(event, window);
    backBtn.handleEvent(event, window);
    leftBtn.handleEvent(event, window);
    rightBtn.handleEvent(event, window);

    if (event.type == sf::Event::KeyPressed) {
        // P1: A/D
        if (event.key.code == sf::Keyboard::A) {
            p1Selection = (p1Selection - 1 + 4) % 4;
            updateArrowPosition(1);
        }
        if (event.key.code == sf::Keyboard::D) {
            p1Selection = (p1Selection + 1) % 4;
            updateArrowPosition(1);
        }

        // P2: Left/Right arrows
        if (isMultiplayer) {
            if (event.key.code == sf::Keyboard::Left) {
                p2Selection = (p2Selection - 1 + 4) % 4;
                updateArrowPosition(2);
            }
            if (event.key.code == sf::Keyboard::Right) {
                p2Selection = (p2Selection + 1) % 4;
                updateArrowPosition(2);
            }
        }

        // Enter to start
        if (event.key.code == sf::Keyboard::Enter) {
            onButtonClick("start");
        }
    }
}

void CharacterSelect::update(float dt) {}

void CharacterSelect::draw(sf::RenderWindow& window) {
    window.draw(blockTexture);
    window.draw(titleText);

    for (int i = 0; i < 4; i++) {
        window.draw(charSprites[i]);
        window.draw(charNameTexts[i]);
    }

    window.draw(p1Arrow);
    window.draw(p1Label);
    if (isMultiplayer) {
        window.draw(p2Arrow);
        window.draw(p2Label);
    }

    startBtn.draw(window);
    backBtn.draw(window);
}

void CharacterSelect::onButtonClick(const std::string& buttonId) {
    if (buttonId == "left") {
        if (isMultiplayer) {
            p2Selection = (p2Selection - 1 + 4) % 4;
            updateArrowPosition(2);
        }
        else {
            p1Selection = (p1Selection - 1 + 4) % 4;
            updateArrowPosition(1);
        }
    }
    else if (buttonId == "right") {
        if (isMultiplayer) {
            p2Selection = (p2Selection + 1) % 4;
            updateArrowPosition(2);
        }
        else {
            p1Selection = (p1Selection + 1) % 4;
            updateArrowPosition(1);
        }
    }
    else if (buttonId == "start") {
        confirmed = true;
        sceneManager->closeDialog();

        GameState* gs = new GameState();
        LevelLoader* loader = new LevelLoader();
        loader->init("./assets/levels/levels.cfg");
        gs->setLevelLoader(loader);
        gs->setPlayerMode(isMultiplayer ? 2 : 1);
        gs->setCharacterSelection(p1Selection, p2Selection);
        gs->setStartLevel(startLevel);
        sceneManager->changeScene(gs);
    }
    else if (buttonId == "back") {
        confirmed = false;
        sceneManager->closeDialog();
    }
}