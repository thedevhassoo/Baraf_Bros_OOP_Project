#pragma once
#include<SFML/Graphics.hpp>
#include "TextureButton.h"
#include "Scene.h"
class Dialog : public Scene
{
protected:
	sf::RectangleShape blockTexture;
	sf::Texture dialogTexture;
	sf::Sprite dialogSprite;
public:
	Dialog();
};

