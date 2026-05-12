#pragma once
#include "Dialog.h"
#include "TextureButton.h"

class SettingsDialog : public Dialog {
private:
    TextureButton muteBtn;
    TextureButton closeBtn;
    bool isMuted;

public:
    SettingsDialog();
    ~SettingsDialog();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void onButtonClick(const std::string& buttonId) override;
};