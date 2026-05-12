#include "GameOverScene.h"
#include "SceneManager.h"
#include "MenuScene.h"

GameOverScene::GameOverScene() {
    bgTexture.loadFromFile("./assets/menu/bg.png");
    bgSprite.setTexture(bgTexture);
    font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");

    titleText.setFont(font);
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Red);
    titleText.setString("GAME OVER");
    titleText.setPosition(450, 200);

    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(450, 280);

    levelText.setFont(font);
    levelText.setCharacterSize(28);
    levelText.setFillColor(sf::Color::Yellow);
    levelText.setPosition(450, 330);

    retryButton = new TextureButton("./assets/menu/play_btn0001.png",
        "./assets/menu/play_btn0002.png",
        this, "retryButton");
    retryButton->setPos({ 500, 420 });

    menuButton = new TextureButton("./assets/menu/exit_btn0001.png",
        "./assets/menu/exit_btn0002.png",
        this, "menuButton");
    menuButton->setPos({ 650, 420 });
}

GameOverScene::~GameOverScene() {
    delete retryButton;
    delete menuButton;
}

void GameOverScene::setResults(int finalScore, int levelReached) {
    scoreText.setString("Final Score: " + std::to_string(finalScore));
    levelText.setString("Level Reached: " + std::to_string(levelReached));
}

void GameOverScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    retryButton->handleEvent(event, window);
    menuButton->handleEvent(event, window);
}

void GameOverScene::update(float dt) {}

void GameOverScene::draw(sf::RenderWindow& window) {
    window.draw(bgSprite);
    window.draw(titleText);
    window.draw(scoreText);
    window.draw(levelText);
    retryButton->draw(window);
    menuButton->draw(window);
}

void GameOverScene::onButtonClick(const std::string& buttonId) {
    if (buttonId == "retryButton") {
        // Restart game
    }
    else if (buttonId == "menuButton") {
        sceneManager->changeScene(new MenuScene());
    }
}