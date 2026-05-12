#include "NewGameDialog.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "AuthManager.h"
#include "GameState.h"
#include "LevelLoader.h"
#include "CharacterSelect.h"

NewGameDialog::NewGameDialog(int players)
    : playerCount(players), newGameBtn("./assets/menu/newgamebtn.png", this, "newGame"),
    continueBtn("./assets/menu/continuebtn.png", this, "continueGame"),
    closeBtn("./assets/leaderboard/close_btn0001.png",
        "./assets/leaderboard/close_btn0002.png",
        "./assets/leaderboard/close_btn0003.png", this, "closeButton") {

    blockTexture.setSize(sf::Vector2f(1280, 720));
    blockTexture.setFillColor(sf::Color(0, 0, 0, 200));

    newGameBtn.setPos({ 515.9, 338 });
    continueBtn.setPos({ 515.9, 373.05 });
    closeBtn.setPos({ 1150.2, 30 });
	std::cout << playerCount << " player(s) selected for new game dialog." << std::endl;
}

NewGameDialog::~NewGameDialog() {}

void NewGameDialog::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    newGameBtn.handleEvent(event, window);
    continueBtn.handleEvent(event, window);
    closeBtn.handleEvent(event, window);
}

void NewGameDialog::update(float dt) {}

void NewGameDialog::draw(sf::RenderWindow& window) {
    window.draw(blockTexture);
    newGameBtn.draw(window);
    continueBtn.draw(window);
    closeBtn.draw(window);
}

void NewGameDialog::onButtonClick(const std::string& buttonId) {
	std::cout << "Button clicked: " << buttonId << std::endl;
    if (buttonId == "newGame") {
        sceneManager->closeDialog();
        CharacterSelect* cs = new CharacterSelect();
        cs->setMultiplayer(playerCount == 2);
        sceneManager->openDialog(cs);
    }
    else if (buttonId == "continueGame") {
        sceneManager->closeDialog();

        int savedLevel = 1;
        if (sceneManager->gameManager->authManager->isLoggedIn()) {
            savedLevel = sceneManager->gameManager->authManager->getLevel();
        }
        if (savedLevel < 1) savedLevel = 1;

        CharacterSelect* cs = new CharacterSelect();
        cs->setMultiplayer(playerCount == 2);
        cs->setContinueMode(true);
        cs->setStartLevel(savedLevel);
        sceneManager->openDialog(cs);
    }
    else if (buttonId == "closeButton") {
        sceneManager->closeDialog();
    }
}