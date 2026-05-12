#pragma once
#include "Dialog.h"
#include "TextureButton.h"

class NewGameDialog : public Dialog {
private:
    TextureButton newGameBtn;
    TextureButton continueBtn;
    TextureButton closeBtn;
    int playerCount;
public:
    NewGameDialog(int players);
    ~NewGameDialog();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
};