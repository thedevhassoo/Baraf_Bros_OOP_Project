#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "TextureButton.h"

class GameOverScene : public Scene {
private:
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Font font;

    sf::Text titleText;
    sf::Text scoreText;
    sf::Text levelText;

    TextureButton* retryButton;
    TextureButton* menuButton;

public:
    GameOverScene();
    ~GameOverScene();

    void setResults(int finalScore, int levelReached);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
};