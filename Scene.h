#pragma once
#include <SFML/Graphics.hpp>

class SceneManager;
class GameManager;

class Scene {
protected:
    SceneManager* sceneManager;
public:
    virtual ~Scene(){}

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onButtonClick(const std::string& buttonId) {}
    void setSceneManager(SceneManager* manager);
};