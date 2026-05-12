#include "playDialog.h"
#include "SceneManager.h"
#include "LevelEditor.h"
//#include "GameState.h"
#include "LevelLoader.h"
//#include "CharacterSelect.h"
#include "NewGameDialog.h"
playDialog::playDialog() :closeButton("./assets/leaderboard/close_btn0001.png", "./assets/leaderboard/close_btn0002.png", "./assets/leaderboard/close_btn0003.png", this, "closeButton"),
singlePlayerButton("./assets/menu/singleplayer_btn0001.png","./assets/menu/singleplayer_btn0002.png","./assets/menu/singleplayer_btn0003.png", this, "singlePlayerButton"),
multiPlayerButton("./assets/menu/multiplayer_btn0001.png","./assets/menu/multiplayer_btn0002.png","./assets/menu/multiplayer_btn0003.png", this, "multiPlayerButton"),
levelEditorButton("./assets/menu/builder_btn.png", this, "levelEditorButton")
{
    float startX = 367.1, startY = 87;
    float height = 55.0;
    dialogTexture.loadFromFile("./assets/menu/chooseDialog.png");
    dialogSprite.setTexture(dialogTexture);
    closeButton.setPos({ 866.85, 39.7 });
    dialogSprite.setPosition({ (1280 / 2) - (dialogTexture.getSize().x / 2.0f), (720 / 2) - (dialogTexture.getSize().y / 2.0f) });
    closeButton.setPos({ dialogSprite.getPosition().x + 428.2f, dialogSprite.getPosition().y + 10.f });
    singlePlayerButton.setPos({ dialogSprite.getPosition().x + 13.25f, dialogSprite.getPosition().y + 67.f });
    multiPlayerButton.setPos({ dialogSprite.getPosition().x + 172.35f, dialogSprite.getPosition().y + 67.f });
    levelEditorButton.setPos({ dialogSprite.getPosition().x + 333.25f, dialogSprite.getPosition().y + 67.f });
    //closeButton.currentScene = this;
    std::cout << "closeButton currentScene = " << closeButton.getCurrentScene() << std::endl;
    std::cout << "playDialog:: constructor called with manager currently = " << sceneManager << std::endl;
}



playDialog::~playDialog()
{
    
}

void playDialog::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    closeButton.handleEvent(event, window);
	singlePlayerButton.handleEvent(event, window);
    	multiPlayerButton.handleEvent(event, window);
            	levelEditorButton.handleEvent(event, window);
    // Handle other events if necessary (e.g., clicking on leaderboard items)
}

void playDialog::update(float dt)
{
}

void playDialog::draw(sf::RenderWindow& window)
{
    window.draw(blockTexture);
    window.draw(dialogSprite);
    closeButton.draw(window);
	singlePlayerButton.draw(window);
    	multiPlayerButton.draw(window);
                    	levelEditorButton.draw(window);
   
}

void playDialog::onButtonClick(const std::string& buttonId) {
	std::cout << "playDialog::onButtonClick called with buttonId: " << buttonId << std::endl;
    if (buttonId == "closeButton") {
        sceneManager->closeDialog();
    }
    else if (buttonId == "singlePlayerButton") {
        sceneManager->openDialog(new NewGameDialog(1));
    }
    else if (buttonId == "multiPlayerButton") {
        sceneManager->openDialog(new NewGameDialog(2));
    }
    else if (buttonId == "levelEditorButton") {
        sceneManager->changeScene(new LevelEditor());
    }
}

