#pragma once
#include<SFML/Graphics.hpp>
#include "Scene.h"
#include "TextureButton.h"
#include "TextBox.h"
class MenuScene:public Scene
{
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	TextureButton playButton;
	TextureButton shopButton;
	TextureButton leaderboardButton;
	TextureButton exitButton;
	TextureButton settingsButton;
	sf::Music menuMusic;
	bool musicStarted;
public:
	MenuScene();
	void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	void onEnter() override;
	void onExit() override;
	void onButtonClick(const std::string& buttonId) override;

};

