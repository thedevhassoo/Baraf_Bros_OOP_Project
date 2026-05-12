#include "HUD.h"
#include <string>

HUD::HUD()
    : fontLoaded(false), hudBgLoaded(false), twoPlayerMode(false),
    powerUpTimer(0), showingPowerUp(false) {

    bossHealthBg.setSize(sf::Vector2f(300, 18));
    bossHealthBg.setFillColor(sf::Color(50, 50, 50));
    bossHealthBg.setOutlineColor(sf::Color::White);
    bossHealthBg.setOutlineThickness(1);

    bossHealthFill.setSize(sf::Vector2f(300, 18));
    bossHealthFill.setFillColor(sf::Color::Red);

    bossNameText.setCharacterSize(16);
    bossNameText.setFillColor(sf::Color::White);

    overlayText.setCharacterSize(48);
    overlayText.setFillColor(sf::Color::Yellow);

    overlaySubText.setCharacterSize(28);
    overlaySubText.setFillColor(sf::Color::White);
}

HUD::~HUD() {}

bool HUD::init() {
    fontLoaded = font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");
    if (!fontLoaded) return false;

    hudBgLoaded = hudBgTexture.loadFromFile("./assets/hud/hud_bg.png");
    if (hudBgLoaded) {
        hudBgSprite.setTexture(hudBgTexture);
        hudBgSprite.setPosition(0, 0);
    }

    scoreValue.setFont(font);
    scoreValue.setCharacterSize(18);
    scoreValue.setFillColor(sf::Color::White);
    scoreValue.setStyle(sf::Text::Bold);
    scoreValue.setPosition(129.3f, 5.0f);

    gemValue.setFont(font);
    gemValue.setCharacterSize(18);
    gemValue.setFillColor(sf::Color::White);
    gemValue.setStyle(sf::Text::Bold);
    gemValue.setPosition(410.8f, 3.75f);

    levelValue.setFont(font);
    levelValue.setCharacterSize(18);
    levelValue.setFillColor(sf::Color::Yellow);
    levelValue.setStyle(sf::Text::Bold);
    levelValue.setPosition(773.15f, 21.2f);

    livesP1Text.setFont(font);
    livesP1Text.setCharacterSize(18);
    livesP1Text.setFillColor(sf::Color::Cyan);
    livesP1Text.setStyle(sf::Text::Bold);
    livesP1Text.setPosition(1031.95f, 3.75f);

    livesP2Text.setFont(font);
    livesP2Text.setCharacterSize(18);
    livesP2Text.setFillColor(sf::Color::Red);
    livesP2Text.setStyle(sf::Text::Bold);
    livesP2Text.setPosition(1181.15f, 2.7f);

    bossNameText.setFont(font);
    overlayText.setFont(font);
    overlaySubText.setFont(font);
    powerUpText.setFont(font);

    bossHealthBg.setPosition(490, 692);
    bossHealthFill.setPosition(490, 692);
    bossNameText.setPosition(490, 674);

    overlayText.setPosition(400, 280);
    overlaySubText.setPosition(420, 350);

    powerUpText.setCharacterSize(18);
    powerUpText.setFillColor(sf::Color::Green);
    powerUpText.setPosition(10, 108);

    inventoryText.setFont(font);
    inventoryText.setCharacterSize(16);
    inventoryText.setFillColor(sf::Color::Yellow);
	inventoryText.setOutlineColor(sf::Color::Black);
	inventoryText.setOutlineThickness(2);
    inventoryText.setPosition(10, 60);

    return true;
}

void HUD::setScore(int s) {
    scoreValue.setString(std::to_string(s));
}
void HUD::setInventory(int speed, int snowball, int range, int balloon, int lives) {
    inventoryText.setString(
        "1:Spd x" + std::to_string(speed) +
        " 2:Pwr x" + std::to_string(snowball) +
        " 3:Rng x" + std::to_string(range) +
        " 4:Bal x" + std::to_string(balloon) +
        " 5:Life x" + std::to_string(lives)
    );
}
void HUD::setGems(int g) {
    gemValue.setString(std::to_string(g));
}

void HUD::setLevel(int curr, int total) {
    levelValue.setString("Level " + std::to_string(curr));
}

void HUD::setLivesP1(int lives) {
    livesP1Text.setString("Lives: " + std::to_string(lives));
}

void HUD::setLivesP2(int lives) {
    livesP2Text.setString("Lives: " + std::to_string(lives));
}

void HUD::setTwoPlayer(bool mode) {
    twoPlayerMode = mode;
}

void HUD::showPowerUp(const char* name, float duration) {
    powerUpText.setString(name);
    showingPowerUp = true;
    powerUpTimer = duration;
}

void HUD::showBossHealth(const char* name, int current, int max) {
    bossNameText.setString(name);
    float percent = (float)current / (float)max;
    if (percent < 0) percent = 0;
    bossHealthFill.setSize(sf::Vector2f(300.0f * percent, 18.0f));
    if (percent < 0.3f) bossHealthFill.setFillColor(sf::Color(255, 30, 30));
    else if (percent < 0.6f) bossHealthFill.setFillColor(sf::Color(255, 150, 30));
    else bossHealthFill.setFillColor(sf::Color(200, 0, 0));
}

void HUD::hideBossHealth() {
    bossNameText.setString("");
    bossHealthFill.setSize(sf::Vector2f(0, 18));
}

void HUD::showLevelComplete(int scoreEarned, int gemsCollected) {
    overlayText.setString("LEVEL COMPLETE!");
    overlaySubText.setString("+" + std::to_string(scoreEarned) + " points  +" + std::to_string(gemsCollected) + " gems");
}

void HUD::showGameOver(int finalScore, int levelReached) {
    overlayText.setString("GAME OVER");
    overlaySubText.setString("Score: " + std::to_string(finalScore) + "  Level: " + std::to_string(levelReached));
}

void HUD::hideOverlay() {
    overlayText.setString("");
    overlaySubText.setString("");
}

void HUD::update(float dt) {
    if (showingPowerUp) {
        powerUpTimer -= dt;
        if (powerUpTimer <= 0) {
            showingPowerUp = false;
            powerUpText.setString("");
        }
    }
}

void HUD::draw(sf::RenderWindow& window) {
    if (hudBgLoaded) window.draw(hudBgSprite);
    window.draw(scoreValue);
    window.draw(gemValue);
    window.draw(levelValue);
    window.draw(livesP1Text);
    if (twoPlayerMode) window.draw(livesP2Text);
    if (showingPowerUp) window.draw(powerUpText);
    window.draw(inventoryText);
}

void HUD::drawBossHealth(sf::RenderWindow& window) {
    if (bossNameText.getString().getSize() > 0) {
        window.draw(bossNameText);
        window.draw(bossHealthBg);
        window.draw(bossHealthFill);
    }
}

void HUD::drawOverlay(sf::RenderWindow& window) {
    if (overlayText.getString().getSize() > 0) {
        sf::RectangleShape bg(sf::Vector2f(1280, 720));
        bg.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(bg);
        window.draw(overlayText);
        window.draw(overlaySubText);
    }
}

void HUD::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
}