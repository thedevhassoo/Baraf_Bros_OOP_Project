#pragma once
#include <string>

class AuthManager {
private:
    std::string currentUsername;
    std::string currentPasswordHash;
    int currentUserId;
    int currentScore;
    int currentGems;
    int currentBalloons;
    int currentDistancePUP;
    int currentSpeedBoosts;
    int currentSecondChanceChips;
    std::string currentCreatedAt;
    int currentLevel;
    bool loggedIn;
    static int nextUserId;
    
    std::string simpleHash(const char* password);
    std::string getCurrentTimestamp();
    bool findAndLoadUser(const char* username, const char* passwordHash);
    bool usernameExists(const char* username);
    void saveCurrentUser();
    void appendNewUser(const char* username, const char* passwordHash);
    void updateUserInFile();
    int getNextUserId();
    void incrementNextUserId();
    
public:
    AuthManager();
    ~AuthManager();
    void updateLevel(int level);
    bool registerUser(const char* username, const char* password);
    bool loginUser(const char* username, const char* password);
    void logout();
    bool isLoggedIn();
    // Get current user data
    int getLevel();
    int getUserId();
    const char* getUsername();
    int getScore();
    int getGems();
    int getBalloons();
    int getDistancePUP();
    int getSpeedBoosts();
    int getSecondChanceChips();
    const char* getCreatedAt();
    
    // Update current user data
    bool updateScore(int newScore);
    bool updateGems(int newGems);
    bool updateBalloons(int newBalloons);
    bool updateDistancePUP(int newDistancePUP);
    bool updateSpeedBoosts(int newSpeedBoosts);
    bool updateSecondChanceChips(int newChips);
    
    // Add/remove operations
    bool addGems(int amount);
    bool removeGems(int amount);
    bool addScore(int amount);
    bool addBalloons(int amount);
    bool addSpeedBoost(int amount);
    bool addSecondChance(int amount);
    void addDistancePUP(int amount);
    
    // Shop purchases
    bool buySpeedBoost(int gemCost);
    bool buySecondChanceChip(int gemCost);
    bool buyBalloons(int amount, int gemCost);
    bool buyDistancePUP(float amount, int gemCost);

    int stringToInt(const std::string& str);
};



