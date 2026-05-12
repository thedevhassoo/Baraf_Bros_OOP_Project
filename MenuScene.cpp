#include "MenuScene.h"
//#include "Dialog.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "AuthManager.h"
#include "LeaderboardDialog.h"
#include "Shop.h"
#include "playDialog.h"
#include "SettingsDialog.h"
MenuScene::MenuScene() :playButton("./assets/menu/play_btn0001.png", "./assets/menu/play_btn0002.png", this, "playButton"), shopButton("./assets/menu/shop_btn0001.png", "./assets/menu/shop_btn0002.png", this, "shopButton"), leaderboardButton("./assets/menu/leaderboard_btn0001.png", "./assets/menu/leaderboard_btn0002.png", this, "leaderboardButton"), exitButton("./assets/menu/exit_btn0001.png", "./assets/menu/exit_btn0002.png", this, "exitButton"), settingsButton("./assets/menu/settings_btn.png", this, "settingsButton")
{
	musicStarted = false;
	bgTexture.loadFromFile("./assets/menu/bg.png");
	bgSprite.setTexture(bgTexture);
	playButton.setPos(sf::Vector2f(853.8, 233.35));
	shopButton.setPos(sf::Vector2f(843.6, 324.5));
	leaderboardButton.setPos(sf::Vector2f(833.3, 414));
	exitButton.setPos(sf::Vector2f(818.7, 506));
	settingsButton.setPos(sf::Vector2f(1164.7, 667));

}

void MenuScene::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	playButton.handleEvent(event, window);
	shopButton.handleEvent(event, window);
	leaderboardButton.handleEvent(event, window);
	exitButton.handleEvent(event, window);
	settingsButton.handleEvent(event, window);
}

void MenuScene::update(float dt)
{
}

void MenuScene::draw(sf::RenderWindow& window)
{
	window.draw(bgSprite);
	playButton.draw(window);
	shopButton.draw(window);
	leaderboardButton.draw(window);
	exitButton.draw(window);
	settingsButton.draw(window);
}

void MenuScene::onEnter() {
	if (!musicStarted) {
		menuMusic.openFromFile("./assets/audio/menu.wav");
		menuMusic.setLoop(true);
		menuMusic.setVolume(70);
		menuMusic.play();
		musicStarted = true;
	}
}

// MenuScene::onExit()
void MenuScene::onExit() {
	menuMusic.stop();
}
void MenuScene::onButtonClick(const std::string& buttonId)
{
	std::cout << "MenuScene: Button clicked: " << buttonId << std::endl;
	if (buttonId == "playButton") {
		sceneManager->openDialog(new playDialog());
	}
	else if (buttonId == "shopButton") {
		Shop* shop = new Shop();
		// Pass ShopSystem from GameManager
		if (sceneManager && sceneManager->gameManager) {
			ShopSystem* ss = new ShopSystem();
			ss->setAuthManager(sceneManager->gameManager->authManager);
			// Player may not exist in menu, so power-ups won't apply instantly
			// They'll be stored in auth for next game session
			shop->setShopSystem(ss);
		}
		sceneManager->openDialog(shop);
	}
	else if (buttonId == "leaderboardButton") {
		sceneManager->openDialog(new LeaderboardDialog());
	}
	else if (buttonId == "exitButton") {
		//Exit the game
		sceneManager->exitGame();
	
	}
	else if (buttonId == "settingsButton") {
		sceneManager->openDialog(new SettingsDialog());
	}
	/*else {
		if (sceneManager->isDialogOpen && sceneManager->currentDialog) {
			sceneManager->currentDialog->onButtonClick(buttonId);
		}
	}*/
}
