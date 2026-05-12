#pragma once
#include <SFML/Graphics.hpp>

class HUD {
private:
    sf::Font font;
    bool fontLoaded;

    sf::Texture hudBgTexture;
    sf::Sprite hudBgSprite;
    bool hudBgLoaded;

    sf::Text scoreValue;
    sf::Text gemValue;
    sf::Text levelValue;
    sf::Text livesP1Text;
    sf::Text livesP2Text;
    bool twoPlayerMode;

    sf::Text powerUpText;
    float powerUpTimer;
    bool showingPowerUp;

    sf::RectangleShape bossHealthBg;
    sf::RectangleShape bossHealthFill;
    sf::Text bossNameText;

    sf::Text overlayText;
    sf::Text overlaySubText;
    sf::Text inventoryText;
public:
    HUD();
    ~HUD();
    void setInventory(int speed, int snowball, int range, int balloon, int lives);
    bool init();

    void setScore(int score);
    void setGems(int gems);
    void setLevel(int current, int total);
    void setLivesP1(int lives);
    void setLivesP2(int lives);
    void setTwoPlayer(bool mode);

    void showPowerUp(const char* name, float duration);
    void showBossHealth(const char* bossName, int current, int max);
    void hideBossHealth();
    void showLevelComplete(int scoreEarned, int gemsCollected);
    void showGameOver(int finalScore, int levelReached);
    void hideOverlay();

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void drawBossHealth(sf::RenderWindow& window);
    void drawOverlay(sf::RenderWindow& window);

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
};