#include "SceneManager.h"
#include "Scene.h"
#include "GameManager.h"

SceneManager::SceneManager()
    : currentScene(nullptr)
    , nextScene(nullptr)
    , shouldChangeScene(false)
    , gameManager(new GameManager())
    , currentDialog(nullptr)
    , isDialogOpen(false)
{
	//gameManager = new GameManager();
}

SceneManager::~SceneManager() {
    delete currentScene;
    delete nextScene;
    delete gameManager;
    if(currentDialog)
    	delete currentDialog;
    
}
void SceneManager::setWindow(sf::RenderWindow* win) { window = win; }
void SceneManager::exitGame() { if (window) window->close(); }
void SceneManager::changeScene(Scene* scene) {
    nextScene = scene;
    shouldChangeScene = true;
}

void SceneManager::setScene(Scene* scene) {
    if (currentScene) {
        currentScene->onExit();
        delete currentScene;
    }
    currentScene = scene;
    if (currentScene) {
        std::cout << "setSceneManager called with manager from sceneManager = " << this << std::endl;
        currentScene->setSceneManager(this);
        currentScene->onEnter();
    }
}


void SceneManager::update(float dt) {
    if (shouldChangeScene && nextScene) {
        if (isDialogOpen) {
			closeDialog();
        }
        setScene(nextScene);
        nextScene = nullptr;
        shouldChangeScene = false;
    }

    if (currentScene) {
        currentScene->update(dt);
    }
    cleanupClosedDialog();
    if (isDialogOpen && currentDialog) {
        currentDialog->update(dt);
    }
}

void SceneManager::draw(sf::RenderWindow& window) {
    if (currentScene) {
        currentScene->draw(window);
    }
    if (isDialogOpen && currentDialog) {
        currentDialog->draw(window);
    }
}

void SceneManager::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (isDialogOpen && currentDialog) {
        currentDialog->handleEvent(event, window);
    }
    else if (currentScene) {
        currentScene->handleEvent(event, window);
    }
}

void SceneManager::openDialog(Dialog* dialog)
{
	currentDialog = dialog;
	isDialogOpen = true;
    if (currentDialog) {
        std::cout << "setSceneManager called with manager from sceneManager = " << this << std::endl;
        currentDialog->setSceneManager(this);
		//currentDialog->onEnter();
    }
}

void SceneManager::closeDialog()
{
	isDialogOpen = false;
}
void SceneManager::cleanupClosedDialog()
{
    if (!isDialogOpen && currentDialog) {
        delete currentDialog;
        currentDialog = nullptr;
    }
}

