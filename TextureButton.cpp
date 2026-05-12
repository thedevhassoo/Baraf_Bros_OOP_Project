#include "TextureButton.h"
#include "Scene.h"

static sf::SoundBuffer clickBuf;
static sf::SoundBuffer hoverBuf;
static sf::Sound clickSfx;
static sf::Sound hoverSfx;
static bool soundsLoaded = false;

static void loadButtonSounds() {
	if (soundsLoaded) return;
	clickBuf.loadFromFile("./assets/audio/click.wav");
	clickSfx.setBuffer(clickBuf);
	hoverBuf.loadFromFile("./assets/audio/hover.wav");
	hoverSfx.setBuffer(hoverBuf);
	soundsLoaded = true;
}


TextureButton::TextureButton(std::string path, Scene* scene, std::string buttonName)
{
	//clicked = false;
	loadButtonSounds();
		textures[0].loadFromFile(path);
		textures[1] = textures[0];
		textures[2] = textures[0];
		buttonId = buttonName;
		currentScene = scene;
	
		button.setTexture(textures[0]);
}
TextureButton::TextureButton(std::string path,std::string path_one, Scene* scene, std::string buttonName)
{
	//clicked = false;
	loadButtonSounds();
		textures[0].loadFromFile(path);
		button.setTexture(textures[0]);
		textures[1].loadFromFile(path_one);
		textures[2] = textures[0];
		currentScene = scene;
		buttonId = buttonName;
}
TextureButton::TextureButton(std::string path, std::string path_one, std::string path_two, Scene* scene, std::string buttonName)
{
	//clicked = false;
	loadButtonSounds();
	textures[0].loadFromFile(path);
	button.setTexture(textures[0]);
	textures[1].loadFromFile(path_one);
	textures[2].loadFromFile(path_two);
	currentScene = scene;
	std::cout << currentScene;
	buttonId = buttonName;
}
sf::Vector2f TextureButton::getPosition() const {
	return button.getPosition();
}
void TextureButton::setPos(sf::Vector2f pos) {
	button.setPosition(pos);
}

/*bool TextureButton::isMouseOver(sf::RenderWindow& window)
{
	float mouseX = sf::Mouse::getPosition(window).x;
	float mouseY = sf::Mouse::getPosition(window).y;

	if (button.getGlobalBounds().contains(mouseX, mouseY)) {
		return true;
	}
	else {
		return false;
	}

}
bool TextureButton::isMouseDown() {
	return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}
void TextureButton::OnClick(void (*callback)()) {
	onClickCallback = callback;
}
void TextureButton::update(sf::RenderWindow& window)
{
	if (isMouseOver(window) && !isMouseDown() && clicked) {
		clicked = false;
		if (onClickCallback) {
			onClickCallback();
		}
		//std::cout << "Clicked" << std::endl;
	}
	if (isMouseOver(window) && isMouseDown()) {
		clicked = true;
		button.setTexture(textures[2]);
	}
	else if (isMouseOver(window)) {
		button.setTexture(textures[1]);
	}
	else {
		clicked = false;
		button.setTexture(textures[0]);
	}
	window.draw(button);

}*/

void TextureButton::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
	if (event.type == sf::Event::MouseMoved) {
		sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
		isHovered = button.getGlobalBounds().contains(mousePos.x, mousePos.y);
		if (isHovered) {
			hoverSfx.play();
		}
	}

	if (event.type == sf::Event::MouseButtonPressed && isHovered) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			clickSfx.play();
			isPressed = true;
		}
	}

	if (event.type == sf::Event::MouseButtonReleased && isPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (isHovered && currentScene && !buttonId.empty()) {

					currentScene->onButtonClick(buttonId);
				
			}
			isPressed = false;
		}
	}
}

void TextureButton::draw(sf::RenderWindow& window) {
	if (isPressed) {
		button.setTexture(textures[2]);
	}
	else if (isHovered) {
		button.setTexture(textures[1]);
	}
	else {
		button.setTexture(textures[0]);
	}
	window.draw(button);
}
