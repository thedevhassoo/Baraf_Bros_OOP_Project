#include "GameManager.h"
#include "AuthManager.h"

void GameManager::setMuted(bool mute) {
    muted = mute;
    // Apply to all sounds globally
    sf::Listener::setGlobalVolume(mute ? 0.0f : savedVolume);

}

bool GameManager::isMuted() const {
    return muted;

}

GameManager::GameManager()
{
	players = 0;
	authManager = new AuthManager();
    muted = false;
    savedVolume = 100.0f;

}
GameManager::~GameManager()
{
	delete authManager;
}