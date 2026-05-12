#pragma once
#include<SFML/Graphics.hpp>
#include "Scene.h"
#include "TextureButton.h"
#include "TextBox.h"
class LoginScreen: public Scene
{
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	sf::Font font;
	sf::Text errorText;
	TextureButton loginBtn;
	TextureButton registerBtn;
	TextBox usernameInput;
	TextBox passwordInput;
	void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	void onExit() override;
	void onButtonClick(const std::string& buttonId) override;
public:
	LoginScreen();
};

