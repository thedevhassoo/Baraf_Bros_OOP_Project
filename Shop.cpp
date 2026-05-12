#include "Shop.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "AuthManager.h"

Shop::Shop()
    : shopItems(nullptr), itemCount(5), shopSystem(nullptr),
    closeButton("./assets/shop/close_btn0001.png",
        "./assets/shop/close_btn0002.png",
        "./assets/shop/close_btn0003.png", this, "closeButton") {

    font.loadFromFile("./assets/fonts/Aller_Std_Rg.ttf");

    dialogTexture.loadFromFile("./assets/shop/shop.png");
    dialogSprite.setTexture(dialogTexture);
    closeButton.setPos({ 1056.4f, 140.0f });
    dialogSprite.setPosition({
        (1280.0f / 2.0f) - (dialogTexture.getSize().x / 2.0f),
        (720.0f / 2.0f) - (dialogTexture.getSize().y / 2.0f)
        });

    loadShopItems();

    gemCountText.setFont(font);
    gemCountText.setCharacterSize(22);
    gemCountText.setFillColor(sf::Color::Cyan);
    gemCountText.setPosition(900, 180);
}

Shop::~Shop() {
    for (int i = 0; i < itemCount; ++i) delete shopItems[i];
    delete[] shopItems;
}

void Shop::setShopSystem(ShopSystem* system) {
    shopSystem = system;
    updateGemDisplay();
}

void Shop::updateGemDisplay() {
    if (shopSystem && sceneManager && sceneManager->gameManager) {
        int gems = sceneManager->gameManager->authManager->getGems();
        gemCountText.setString("Your Gems: " + std::to_string(gems));
    }
}

void Shop::loadShopItems() {
    // 5 items matching spec
    const char* names[5] = {
        "Extra Life",
        "Speed Boost",
        "Snowball Power",
        "Range Boost",
        "Flight Mode"
    };
    const char* prices[5] = { "500", "200", "300", "250", "350" };
    const char* ids[5] = {
        "buy_extra_life",
        "buy_speed",
        "buy_snowball",
        "buy_range",
        "buy_balloon"
    };

    itemCount = 5;
    shopItems = new ShopItem * [itemCount];

    float startX = 245.0f, startY = 240.0f;
    float height = 62.0f;

    for (int i = 0; i < itemCount; i++) {
        shopItems[i] = new ShopItem(names[i], ids[i], prices[i], this);
        shopItems[i]->setPos({ startX, startY + (height * i) });
    }
}

void Shop::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    closeButton.handleEvent(event, window);
    for (int i = 0; i < itemCount; i++) {
        shopItems[i]->buyButton.handleEvent(event, window);
    }
}

void Shop::update(float dt) {
    updateGemDisplay();
}

void Shop::draw(sf::RenderWindow& window) {
    window.draw(blockTexture);
    window.draw(dialogSprite);
    closeButton.draw(window);
    for (int i = 0; i < itemCount; i++) {
        shopItems[i]->draw(window);
    }
    window.draw(gemCountText);
}

void Shop::onButtonClick(const std::string& buttonId) {
    if (buttonId == "closeButton") {
        sceneManager->closeDialog();
    }
    else if (buttonId == "buy_extra_life") {
        if (shopSystem && shopSystem->buyExtraLife()) {
            updateGemDisplay();
        }
        else {
			std::cout << "Not enough gems for Extra Life!" << std::endl;
        }
    }
    else if (buttonId == "buy_speed") {
        if (shopSystem && shopSystem->buySpeedBoost()) {
            updateGemDisplay();
        }
        else {
            std::cout << "Not enough gems for Speed Boost!" << std::endl;
        }
    }
    else if (buttonId == "buy_snowball") {
        if (shopSystem && shopSystem->buySnowballPower()) {
            updateGemDisplay();
        }
        else {
            std::cout << "Not enough gems for Snowball Boost!" << std::endl;
        }
    }
    else if (buttonId == "buy_range") {
        if (shopSystem && shopSystem->buyDistanceIncrease()) {
            updateGemDisplay();
        }
        else {
            std::cout << "Not enough gems for Snowball Power!" << std::endl;
        }
    }
    else if (buttonId == "buy_balloon") {
        if (shopSystem && shopSystem->buyBalloonMode()) {
            updateGemDisplay();
        }
        else {
            std::cout << "Not enough gems for Balloon Mode!" << std::endl;
        }
    }
}