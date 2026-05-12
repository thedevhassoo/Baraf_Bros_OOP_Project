#pragma once
#include<SFML/Graphics.hpp>
#include "TextureButton.h"
class Shop;
class ShopItem
{
	sf::Texture	bgTexture;
	sf::Sprite bgSprite;
	sf::Texture itemIconTexture;
	sf::Sprite itemIconSprite;
	sf::Text nameText;
	sf::Text priceText;
	sf::Font font;
	//std::string player;
	//std::string score;
public:
	TextureButton buyButton;
	ShopItem(std::string name, std::string id, std::string price, Shop* shop);
	void draw(sf::RenderWindow& window);
	void setPos(sf::Vector2f pos);
};

