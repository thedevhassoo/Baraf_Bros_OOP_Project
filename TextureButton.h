#pragma once
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
class Scene;
class TextureButton
{
	sf::Texture textures[3];
	sf::Sprite button;
	bool isHovered = false;
	bool isPressed = false;
	std::string buttonId;  // Unique identifier for this button
	Scene* currentScene;   // Pointer to current scene
	//static sf::SoundBuffer clickBuf;
	//static sf::SoundBuffer hoverBuf;
	//static sf::Sound clickSfx;
	//static sf::Sound hoverSfx;
	//static bool soundsLoaded;

public:
	TextureButton(std::string path, Scene* scene, std::string buttonName);
	TextureButton(std::string path, std::string path_one, Scene* scene, std::string buttonName);
	TextureButton(std::string path, std::string path_one, std::string path_two, Scene* scene, std::string buttonName);
	Scene* getCurrentScene() const { return currentScene; }
	sf::Vector2f getPosition() const;
	//void update(float dt);
	void draw(sf::RenderWindow& window);

	//void OnClick(void (*callback)());
	void setPos(sf::Vector2f pos);

	void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
	//void (Scene::*onClickCallback)() = nullptr;

};

