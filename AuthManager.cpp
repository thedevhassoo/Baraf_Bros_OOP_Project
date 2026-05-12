#include "AuthManager.h"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include<iostream>
int AuthManager::nextUserId = 1;

AuthManager::AuthManager() {
    currentLevel = 1;
    loggedIn = false;
    currentUserId = -1;
    currentScore = 0;
    currentGems = 100;
    currentBalloons = 0;
    currentDistancePUP = 1;
    currentSpeedBoosts = 0;
    currentSecondChanceChips = 0;
}

AuthManager::~AuthManager() {
    if (loggedIn) {
        saveCurrentUser();
    }
}
int AuthManager::getLevel() {
    return currentLevel;
}
std::string AuthManager::simpleHash(const char* password) {
    std::string hashed = "";

    for (int i = 0; password[i] != '\0'; i++) {
        int val = password[i] + 3;

        char temp[5];
        int j = 0;

        char rev[5];
        int r = 0;

        if (val == 0) {
            temp[j++] = '0';
        }
        else {
            while (val > 0) {
                rev[r++] = (val % 10) + '0';
                val /= 10;
            }
            for (int k = r - 1; k >= 0; k--) {
                temp[j++] = rev[k];
            }
        }

        temp[j] = '\0';
        hashed += temp;
    }

    return hashed;
}
void AuthManager::updateLevel(int level) {
    currentLevel = level;
    updateUserInFile();
}
std::string AuthManager::getCurrentTimestamp() {
    time_t now = time(0);
    char buff[26];
    ctime_s(buff, sizeof(buff), &now);
    return std::string(buff);
}

int AuthManager::getNextUserId() {
    std::ifstream file("next_id.txt");
    if (file.is_open()) {
        file >> nextUserId;
        file.close();
    }
    return nextUserId;
}

void AuthManager::incrementNextUserId() {
    nextUserId++;
    std::ofstream file("next_id.txt");
    if (file.is_open()) {
        file << nextUserId;
        file.close();
    }
}

bool AuthManager::usernameExists(const char* username) {
    std::ifstream file("users.txt");
    if (!file.is_open()) return false;

    char line[500];
    while (file.getline(line, 500)) {
        char storedUsername[50];
        int i = 0;
        while (line[i] != '|' && line[i] != '\0') {
            storedUsername[i] = line[i];
            i++;
        }
        storedUsername[i] = '\0';

        if (strcmp(storedUsername, username) == 0) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool AuthManager::findAndLoadUser(const char* username, const char* passwordHash) {
    std::ifstream file("users.txt");
    if (!file.is_open()) return false;

    char line[500];
    while (file.getline(line, 500)) {
        // Parse line: username|hash|id|score|gems|balloons|distancePUP|speedBoosts|secondChance|createdAt
        char storedUsername[50];
        char storedHash[500];

        // Extract username
        int i = 0;
        while (line[i] != '|' && line[i] != '\0') {
            storedUsername[i] = line[i];
            i++;
        }
        storedUsername[i] = '\0';
        i++; // skip |

        // Extract hash
        int j = 0;
        while (line[i] != '|' && line[i] != '\0') {
            storedHash[j++] = line[i++];
        }
        storedHash[j] = '\0';

        if (strcmp(storedUsername, username) == 0 && strcmp(storedHash, passwordHash) == 0) {
            // Load all data
            currentUsername = storedUsername;
            currentPasswordHash = storedHash;

            i++; // skip |

            // Parse id
            char idStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                idStr[j++] = line[i++];
            }
            idStr[j] = '\0';
            currentUserId = stringToInt(idStr);
            i++;

            // Parse score
            char scoreStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                scoreStr[j++] = line[i++];
            }
            scoreStr[j] = '\0';
            currentScore = stringToInt(scoreStr);
            i++;

            // Parse gems
            char gemsStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                gemsStr[j++] = line[i++];
            }
            gemsStr[j] = '\0';
            currentGems = stringToInt(gemsStr);
            i++;

            // Parse balloons
            char balloonsStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                balloonsStr[j++] = line[i++];
            }
            balloonsStr[j] = '\0';
            currentBalloons = stringToInt(balloonsStr);
            i++;

            // Parse distancePUP
            char distanceStr[20];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                distanceStr[j++] = line[i++];
            }
            distanceStr[j] = '\0';
            currentDistancePUP = stringToInt(distanceStr);
            i++;

            // Parse speedBoosts
            char speedStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                speedStr[j++] = line[i++];
            }
            speedStr[j] = '\0';
            currentSpeedBoosts = stringToInt(speedStr);
            i++;

            // Parse secondChanceChips
            char secondStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                secondStr[j++] = line[i++];
            }
            secondStr[j] = '\0';
            currentSecondChanceChips = stringToInt(secondStr);
            i++;
            // Parse level
            char levelStr[10];
            j = 0;
            while (line[i] != '|' && line[i] != '\0') {
                levelStr[j++] = line[i++];
            }
            levelStr[j] = '\0';
            currentLevel = stringToInt(levelStr);
            i++;
            // Parse createdAt
            j = 0;
            char createdAtStr[30];
            while (line[i] != '\0') {
                createdAtStr[j++] = line[i++];
            }
            createdAtStr[j] = '\0';
            currentCreatedAt = createdAtStr;

            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void AuthManager::appendNewUser(const char* username, const char* passwordHash) {
    std::ofstream file("users.txt", std::ios::app);
    if (!file.is_open()) return;

    int userId = getNextUserId();
    std::string timestamp = getCurrentTimestamp();

    file << username << "|"
        << passwordHash << "|"
        << userId << "|"
        << "0|"      // score
        << "0|"      // gems
        << "0|"      // balloons
        << "0|"      // distancePUP
        << "0|"      // speedBoosts
        << "0|"      // secondChanceChips
        << "1|"      // currentLevel — start at level 1
        << timestamp << "\n";

    file.close();
    incrementNextUserId();
}

void AuthManager::saveCurrentUser() {
    updateUserInFile();
}

int AuthManager::stringToInt(const std::string& str) {
    int result = 0;
    for (int i = 0; i < str.length(); i++) {
        result = result * 10 + (str[i] - '0');
    }
    return result;
}

void AuthManager::updateUserInFile() {
    std::ifstream infile("users.txt");
    std::ofstream outfile("users_temp.txt");

    if (!infile.is_open() || !outfile.is_open()) return;

    char line[500];
    while (infile.getline(line, 500)) {
        char storedUsername[50];
        int i = 0;
        while (line[i] != '|' && line[i] != '\0') {
            storedUsername[i] = line[i];
            i++;
        }
        storedUsername[i] = '\0';

        if (strcmp(storedUsername, currentUsername.c_str()) == 0) {
            // Write updated data
            outfile << currentUsername << "|"
                << currentPasswordHash << "|"
                << currentUserId << "|"
                << currentScore << "|"
                << currentGems << "|"
                << currentBalloons << "|"
                << currentDistancePUP << "|"
                << currentSpeedBoosts << "|"
                << currentSecondChanceChips << "|"
                << currentLevel << "|"
                << currentCreatedAt << "\n";
        }
        else {
            outfile << line << "\n";
        }
    }

    infile.close();
    outfile.close();

    remove("users.txt");
    rename("users_temp.txt", "users.txt");
}

bool AuthManager::registerUser(const char* username, const char* password) {
    if (usernameExists(username)) {
        return false;
    }

    std::string hash = simpleHash(password);
    appendNewUser(username, hash.c_str());
    return true;
}

bool AuthManager::loginUser(const char* username, const char* password) {
    std::string hash = simpleHash(password);

    if (findAndLoadUser(username, hash.c_str())) {
        loggedIn = true;
        return true;
    }

    return false;
}

void AuthManager::logout() {
    if (loggedIn) {
        saveCurrentUser();
        loggedIn = false;
    }
}

bool AuthManager::isLoggedIn() {
    return loggedIn;
}

int AuthManager::getUserId() {
    return currentUserId;
}

const char* AuthManager::getUsername() {
    return currentUsername.c_str();
}

int AuthManager::getScore() {
    return currentScore;
}

int AuthManager::getGems() {
    return currentGems;
}

int AuthManager::getBalloons() {
    return currentBalloons;
}

int AuthManager::getDistancePUP() {
    return currentDistancePUP;
}

int AuthManager::getSpeedBoosts() {
    return currentSpeedBoosts;
}

int AuthManager::getSecondChanceChips() {
    return currentSecondChanceChips;
}

const char* AuthManager::getCreatedAt() {
    return currentCreatedAt.c_str();
}

bool AuthManager::updateScore(int newScore) {
    currentScore = newScore;
    updateUserInFile();
    return true;
}

bool AuthManager::updateGems(int newGems) {
    currentGems = newGems;
    updateUserInFile();
    return true;
}

bool AuthManager::updateBalloons(int newBalloons) {
    currentBalloons = newBalloons;
    updateUserInFile();
    return true;
}

bool AuthManager::updateDistancePUP(int newDistancePUP) {
    currentDistancePUP = newDistancePUP;
    updateUserInFile();
    return true;
}

bool AuthManager::updateSpeedBoosts(int newSpeedBoosts) {
    currentSpeedBoosts = newSpeedBoosts;
    updateUserInFile();
    return true;
}

bool AuthManager::updateSecondChanceChips(int newChips) {
    currentSecondChanceChips = newChips;
    updateUserInFile();
    return true;
}

bool AuthManager::addGems(int amount) {
    currentGems += amount;
	std::cout << "Added " << amount << " gems. New total: " << currentGems << std::endl;
    updateUserInFile();
    return true;
}

bool AuthManager::removeGems(int amount) {
    if (currentGems >= amount) {
        currentGems -= amount;
        updateUserInFile();
        return true;
    }
    return false;
}

bool AuthManager::addScore(int amount) {
    currentScore += amount;
    updateUserInFile();
    return true;
}

bool AuthManager::addBalloons(int amount) {
    currentBalloons += amount;
    updateUserInFile();
    return true;
}

bool AuthManager::addSpeedBoost(int amount) {
    currentSpeedBoosts += amount;
    updateUserInFile();
    return true;
}

bool AuthManager::addSecondChance(int amount) {
    currentSecondChanceChips += amount;
    updateUserInFile();
    return true;
}

void AuthManager::addDistancePUP(int amount) {
    currentDistancePUP += amount;
    updateUserInFile();
}

bool AuthManager::buySpeedBoost(int gemCost) {
    if (currentGems >= gemCost) {
        currentGems -= gemCost;
        currentSpeedBoosts++;
        updateUserInFile();
        return true;
    }
    return false;
}

bool AuthManager::buySecondChanceChip(int gemCost) {
    if (currentGems >= gemCost) {
        currentGems -= gemCost;
        currentSecondChanceChips++;
        updateUserInFile();
        return true;
    }
    return false;
}

bool AuthManager::buyBalloons(int amount, int gemCost) {
    if (currentGems >= gemCost) {
        currentGems -= gemCost;
        currentBalloons += amount;
        updateUserInFile();
        return true;
    }
    return false;
}

bool AuthManager::buyDistancePUP(float amount, int gemCost) {
    if (currentGems >= gemCost) {
        currentGems -= gemCost;
        currentDistancePUP += amount;
        updateUserInFile();
        return true;
    }
    return false;
}