#include "SettingsDialog.h"
#include "SceneManager.h"
#include "GameManager.h"

SettingsDialog::SettingsDialog()
    : isMuted(false),
    muteBtn("./assets/settings/mutebtn.png", this, "mute"),
    closeBtn("./assets/leaderboard/close_btn0001.png",
        "./assets/leaderboard/close_btn0002.png",
        "./assets/leaderboard/close_btn0003.png", this, "closeButton") {

    blockTexture.setSize(sf::Vector2f(1280, 720));
    blockTexture.setFillColor(sf::Color(0, 0, 0, 180));

    dialogTexture.loadFromFile("./assets/settings/settingsdialog.png");
    dialogSprite.setTexture(dialogTexture);
    dialogSprite.setPosition({
        (1280.0f / 2.0f) - (dialogTexture.getSize().x / 2.0f),
        (720.0f / 2.0f) - (dialogTexture.getSize().y / 2.0f)
        });

    muteBtn.setPos({ 255, 269 });
    closeBtn.setPos({ 993, 195 });
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    muteBtn.handleEvent(event, window);
    closeBtn.handleEvent(event, window);
}

void SettingsDialog::update(float dt) {}

void SettingsDialog::draw(sf::RenderWindow& window) {
    window.draw(blockTexture);
    window.draw(dialogSprite);
    muteBtn.draw(window);
    closeBtn.draw(window);
}

void SettingsDialog::onButtonClick(const std::string& buttonId) {
    if (buttonId == "mute") {
        isMuted = !isMuted;
        if (sceneManager && sceneManager->gameManager) {
            sceneManager->gameManager->setMuted(isMuted);
        }
    }
    else if (buttonId == "closeButton") {
        sceneManager->closeDialog();
    }
}