#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class AuthManager;
class GameManager
{
	int players;
	bool muted;
	float savedVolume;
public:
	AuthManager* authManager;
	void setMuted(bool mute);
	bool isMuted() const;
	GameManager();
	~GameManager();
};

