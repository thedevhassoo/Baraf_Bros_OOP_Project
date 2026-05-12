#include "RegisterScene.h"
#include "LoginScreen.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "AuthManager.h"

void RegisterScene::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
		usernameInput.handleEvent(event, window);
		passwordInput.handleEvent(event, window);
		confirmPasswordInput.handleEvent(event, window);
		createBtn.handleEvent(event, window);
		backBtn.handleEvent(event, window);

}

void RegisterScene::update(float dt)
{
	usernameInput.update(dt);
	passwordInput.update(dt);
	confirmPasswordInput.update(dt);
}

void RegisterScene::draw(sf::RenderWindow& window)
{
	window.draw(bgSprite);
	usernameInput.draw(window);
	passwordInput.draw(window);
	confirmPasswordInput.draw(window);
	createBtn.draw(window);
	backBtn.draw(window);
	window.draw(errorText);
}

void RegisterScene::onExit()
{
}

void RegisterScene::onButtonClick(const std::string& buttonId)
{
	 if (buttonId == "createBtn") {
		std::string username = usernameInput.getStringAsString();
		std::string password = passwordInput.getStringAsString();
		std::string confirm = confirmPasswordInput.getStringAsString();

		if (username.empty() || password.empty()) {
			errorText.setString("Username and password cannot be empty!");
			errorText.setFillColor(sf::Color::Red);
		}
		else if (password != confirm) {
			errorText.setString("Passwords do not match!");
			errorText.setFillColor(sf::Color::Red);
		}
		else if (sceneManager->gameManager->authManager->registerUser(username.c_str(), password.c_str())) {
			sceneManager->changeScene(new LoginScreen());
		}
		else {
			errorText.setString("Registration failed! Username already exists.");
			errorText.setFillColor(sf::Color::Red);
		}
	}
	else if(buttonId == "backBtn") {
		std::cout << "Back button clicked." << std::endl;
		sceneManager->changeScene(new LoginScreen());
	}
}

RegisterScene::RegisterScene():createBtn("./assets/login/create_btn0001.png", "./assets/login/create_btn0002.png", "./assets/login/create_btn0003.png", this, "createBtn"),
backBtn("./assets/login/back_btn0001.png", "./assets/login/back_btn0002.png", "./assets/login/back_btn0003.png", this, "backBtn")
{
	bgTexture.loadFromFile("./assets/login/register_bg.png");
	bgSprite.setTexture(bgTexture);
	font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");

	usernameInput.init(522.55, 337, 275, 32, font, "Username");
	usernameInput.setFontSize(26);

	passwordInput.init(522.5, 411.6, 275, 32, font, "Password");
	passwordInput.setFontSize(26);
	passwordInput.setHidden(true);

	confirmPasswordInput.init(522.5, 487.75, 275, 32, font, "Confirm Password");
	confirmPasswordInput.setFontSize(26);
	confirmPasswordInput.setHidden(true);

	backBtn.setPos(sf::Vector2f(915.45, 259.25));
	createBtn.setPos(sf::Vector2f(531, 533.45));

	errorText.setFont(font);
	errorText.setCharacterSize(18);
	errorText.setFillColor(sf::Color::Red);
	errorText.setStyle(sf::Text::Bold);
	errorText.setPosition(460.5f, 600.0f);
	errorText.setString("");
}

