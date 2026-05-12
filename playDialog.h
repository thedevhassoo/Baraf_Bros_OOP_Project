#pragma once
#include "Dialog.h"
//#include "LeaderboardItem.h"

class playDialog :public Dialog
{
	TextureButton closeButton;
	TextureButton singlePlayerButton;
	TextureButton multiPlayerButton;
	TextureButton levelEditorButton;
	//LeaderboardItem** leaderboardItems;
	//int itemCount;
public:
	playDialog();
	~playDialog();
	void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	void onButtonClick(const std::string& buttonId) override;
};

