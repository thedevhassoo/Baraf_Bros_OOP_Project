// SceneManager.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Dialog.h"
// Forward declare - OK for header
class Scene;
class GameManager;

class SceneManager {
private:
    Scene* currentScene;
    Scene* nextScene;
    bool shouldChangeScene;
    void setScene(Scene* scene);
    sf::RenderWindow* window;
//protected:
public:

    bool isDialogOpen;
	Dialog* currentDialog;
	GameManager* gameManager;
    SceneManager();
    ~SceneManager();
    void setWindow(sf::RenderWindow* win);
    void exitGame();
    void changeScene(Scene* scene);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
	void openDialog(Dialog* dialog);
	void closeDialog();
    void cleanupClosedDialog();
    //void exitGame();
 
};