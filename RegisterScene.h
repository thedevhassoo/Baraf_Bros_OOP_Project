#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include "TextureButton.h"
#include "TextBox.h"
class RegisterScene : public Scene
{
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	sf::Font font;
	sf::Text errorText;
	TextureButton createBtn;
	TextureButton backBtn;
	TextBox usernameInput;
	TextBox passwordInput;
	TextBox confirmPasswordInput;
	void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void draw(sf::RenderWindow& window) override;
	void onExit() override;
	void onButtonClick(const std::string& buttonId) override;
public:
	RegisterScene();
};

