#pragma once
#include<SFML/Graphics.hpp>
class LeaderboardItem
{
	sf::Texture	bgTexture;
	sf::Sprite bgSprite;
	sf::Text placement;
	sf::Text playerText;
	sf::Text scoreText;
	sf::Font font;
	//std::string player;
	//std::string score;
public:
		LeaderboardItem(int placement, std::string name, std::string score	);
		void draw(sf::RenderWindow& window);
		void setPos(sf::Vector2f pos);
};

