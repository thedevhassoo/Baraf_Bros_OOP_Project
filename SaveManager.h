#pragma once

class AuthManager;

class SaveManager {
private:
    AuthManager* auth;

public:
    SaveManager();
    ~SaveManager();

    void setAuthManager(AuthManager* a);

    // Save player state
    bool saveProgress(int level, int lives, int score, int gems);

    // Load player state
    bool loadProgress(int& level, int& lives, int& score, int& gems);

    // Save leaderboard entry
    bool saveLeaderboardEntry(const char* playerName, int score, int levelReached);

    // Get top scores (returns dynamic array, caller deletes)
    struct LeaderboardEntry {
        char name[32];
        int score;
        int levelReached;
        char date[32];
    };
    LeaderboardEntry** getTopScores(int count, int& outCount);
};