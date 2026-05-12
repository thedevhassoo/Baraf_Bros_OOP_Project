#include "LeaderboardItem.h"
//#include<iostream>


LeaderboardItem::LeaderboardItem(int placement, std::string name, std::string score)
{
	if (!bgTexture.loadFromFile("./assets/leaderboard/leaderboard_item.png")) {
		//std::cout << "FAILED: Could not load ./assets/leaderboard/close_btn0001.png" << std::endl;
	}
	bgSprite.setTexture(bgTexture);
	if (!font.loadFromFile("./assets/fonts/AllerDisplay_Std_Rg.ttf")) {
		//std::cout << "FAILED: Could not load ./assets/fonts/arial.ttf" << std::endl;
	}
	this->placement.setFont(font);
	this->placement.setCharacterSize(26);
	this->placement.setString(std::to_string(placement) + ".");
	playerText.setFont(font);
	playerText.setCharacterSize(26);
	playerText.setString(name);
	scoreText.setFont(font);
	scoreText.setCharacterSize(26);
	scoreText.setString(score);
	scoreText.setFillColor(sf::Color(0, 148, 231));
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(1.0f);
}

void LeaderboardItem::draw(sf::RenderWindow& window)
{
	window.draw(bgSprite);
	window.draw(placement);
	window.draw(playerText);
	window.draw(scoreText);
}

void LeaderboardItem::setPos(sf::Vector2f pos)
{
	bgSprite.setPosition(pos);
	placement.setPosition(pos.x + 45, pos.y + 34); // Adjust placement text position
	playerText.setPosition(pos.x + 85, pos.y + 34); // Adjust text position relative to the background sprite
	scoreText.setPosition(pos.x + 381.4, pos.y + 34); // Adjust score text position

}
