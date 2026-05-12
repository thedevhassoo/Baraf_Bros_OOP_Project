#pragma once
#include "Dialog.h"
#include "LeaderboardItem.h"

class LeaderboardDialog :public Dialog
{
	TextureButton closeButton;
	LeaderboardItem** leaderboardItems;
	int itemCount;
	public:
	LeaderboardDialog();
	~LeaderboardDialog();
	void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	void onButtonClick(const std::string& buttonId) override;
	void loadLeaderboard();
};

