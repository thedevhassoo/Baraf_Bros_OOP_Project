#include "ShopItem.h"
#include "Shop.h"
//#include<iostream>


ShopItem::ShopItem(std::string name, std::string id, std::string price, Shop* shop) : buyButton("./assets/shop/buy_btn0001.png", "./assets/shop/buy_btn0002.png", "./assets/shop/buy_btn0003.png", shop, id)
{
	if (!bgTexture.loadFromFile("./assets/shop/shop_item.png")) {
		//std::cout << "FAILED: Could not load ./assets/shop/shop_item.png" << std::endl;
	}
	bgSprite.setTexture(bgTexture);
	if (!font.loadFromFile("./assets/fonts/AllerDisplay_Std_Rg.ttf")) {
		//std::cout << "FAILED: Could not load ./assets/fonts/AllerDisplay_Std_Rg.ttf" << std::endl;
	}
	if (itemIconTexture.loadFromFile("./assets/shop/" + id + ".png")) {
		itemIconSprite.setTexture(itemIconTexture);
	}
	nameText.setFont(font);
	nameText.setCharacterSize(26);
	nameText.setString(name);
	priceText.setFont(font);
	priceText.setCharacterSize(26);
	priceText.setString(price);
	priceText.setFillColor(sf::Color(0, 148, 231));
	priceText.setOutlineColor(sf::Color::Black);
	priceText.setOutlineThickness(1.0f);

}

void ShopItem::draw(sf::RenderWindow& window)
{
	window.draw(bgSprite);
	window.draw(itemIconSprite);
	window.draw(nameText);
	window.draw(priceText);
	buyButton.draw(window);
}

void ShopItem::setPos(sf::Vector2f pos)
{
	bgSprite.setPosition(pos);
	nameText.setPosition(pos.x + 62.55f, pos.y + 9.0f); // Adjust name text position
	priceText.setPosition(pos.x + 600.35f, pos.y + 9.0f); // Adjust price text position
	buyButton.setPos({ pos.x + 670.55f, pos.y + 9.0f }); // Adjust buy button position
	itemIconSprite.setPosition(pos.x + 9.0f, pos.y + 9.0f); // Adjust item icon position

}
