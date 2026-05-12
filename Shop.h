#pragma once
#include "Dialog.h"
#include "ShopItem.h"
#include "ShopSystem.h"

class Shop : public Dialog {
    TextureButton closeButton;
    ShopItem** shopItems;
    int itemCount;
    ShopSystem* shopSystem;
    sf::Text gemCountText;
    sf::Font font;

public:
    Shop();
    ~Shop();
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
    void loadShopItems();
    void setShopSystem(ShopSystem* system);
    void updateGemDisplay();
};