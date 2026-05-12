#pragma once
#include <SFML/Graphics.hpp>
#include "Dialog.h"
#include "TextureButton.h"

class CharacterSelect : public Dialog {
private:
    sf::Font font;

    static const int CHAR_COUNT = 4;
    sf::Texture charTextures[4];
    sf::Sprite charSprites[4];
    char charNames[4][16];
    int p1Selection;
    int p2Selection;
    bool isMultiplayer;
    bool confirmed;

    sf::Text titleText;
    sf::Text p1Label;
    sf::Text p2Label;
    sf::Text charNameTexts[4];

    sf::Sprite p1Arrow;
    sf::Sprite p2Arrow;
    sf::Texture arrowTexture;

    TextureButton startBtn;
    TextureButton backBtn;
    TextureButton leftBtn;
    TextureButton rightBtn;

    void updateArrowPosition(int player);
    bool continueMode;
    int startLevel;
public:
    void setContinueMode(bool mode);
    void setStartLevel(int level);
    CharacterSelect();
    ~CharacterSelect();

    void setMultiplayer(bool mp);
    int getP1Selection();
    int getP2Selection();
    bool isConfirmed();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
};