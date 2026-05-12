#include "LoginScreen.h"
#include "RegisterScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "AuthManager.h"

void LoginScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	usernameInput.handleEvent(event, window);
	passwordInput.handleEvent(event, window);
	loginBtn.handleEvent(event, window);
	registerBtn.handleEvent(event, window);

}

void LoginScreen::update(float dt)
{
	usernameInput.update(dt);
	passwordInput.update(dt);
	//loginBtn.update(dt);
	//registerBtn.update(dt);
}

void LoginScreen::draw(sf::RenderWindow& window)
{
	window.draw(bgSprite);
	usernameInput.draw(window);
	passwordInput.draw(window);
	loginBtn.draw(window);
	registerBtn.draw(window);
	window.draw(errorText);
}

void LoginScreen::onExit()
{
	//delete sceneManager;
}

LoginScreen::LoginScreen() :loginBtn("./assets/login/login_btn0001.png", "./assets/login/login_btn0002.png", "./assets/login/login_btn0003.png", this, "loginBtn"), 
	registerBtn("./assets/login/signup_btn0001.png", "./assets/login/signup_btn0002.png", "./assets/login/signup_btn0003.png", this, "registerBtn")
{
	bgTexture.loadFromFile("./assets/login/login_bg.png");
	bgSprite.setTexture(bgTexture);
	font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");
	usernameInput.init(520.95, 356.1, 275, 32, font, "Username");
	usernameInput.setFontSize(26);

	passwordInput.init(520.95, 434.8, 275, 32, font, "Password");
	passwordInput.setFontSize(26);
	passwordInput.setHidden(true);
	
	loginBtn.setPos(sf::Vector2f(407.5, 498.55));
	registerBtn.setPos(sf::Vector2f(646.9, 498.55));

	errorText.setFont(font);
	errorText.setCharacterSize(18);
	errorText.setFillColor(sf::Color::Red);
	errorText.setStyle(sf::Text::Bold);
	errorText.setPosition(460.5f, 600.0f);
	errorText.setString("");
}
void LoginScreen::onButtonClick(const std::string& buttonId) {
	if (buttonId == "loginBtn") {
		std::string username = usernameInput.getStringAsString();
		std::string password = passwordInput.getStringAsString();

		if (username.empty() || password.empty()) {
			errorText.setString("Please enter username and password!");
			errorText.setFillColor(sf::Color::Red);
		}
		else if (sceneManager->gameManager->authManager->loginUser(username.c_str(), password.c_str())) {
			sceneManager->changeScene(new MenuScene());
		}
		else {
			errorText.setString("Login failed! Invalid username or password.");
			errorText.setFillColor(sf::Color::Red);
		}
	}
	else if (buttonId == "registerBtn") {
		std::cout << "Register button clicked." << std::endl;
		sceneManager->changeScene(new RegisterScene());
	}
}