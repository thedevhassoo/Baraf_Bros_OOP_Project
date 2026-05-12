#include "SaveManager.h"
#include "AuthManager.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <iostream>

SaveManager::SaveManager() : auth(nullptr) {}
SaveManager::~SaveManager() {}

void SaveManager::setAuthManager(AuthManager* a) { auth = a; }

bool SaveManager::saveProgress(int level, int lives, int score, int gems) {
    std::cout << "Saving progress: Level " << level << ", Lives " << lives
        << ", Score " << score << ", Gems " << gems << std::endl;
    if (!auth) {
        std::cout << "Auth is null!" << std::endl;
        return false;
    }

    auth->updateScore(score);
    auth->updateGems(gems);
    auth->updateSecondChanceChips(lives);

    saveLeaderboardEntry(auth->getUsername(), score, level);

    return true;
}

bool SaveManager::loadProgress(int& level, int& lives, int& score, int& gems) {
    if (!auth) return false;
    score = auth->getScore();
    gems = auth->getGems();
    lives = auth->getSecondChanceChips() + 2;
    level = 1;  // Always start fresh or load from player_progress equivalent
    return true;
}

bool SaveManager::saveLeaderboardEntry(const char* playerName, int score, int levelReached) {
    // Read existing entries
    std::ifstream inFile("leaderboard.txt");

    char allLines[100][256];
    int lineCount = 0;

    char line[256];
    while (inFile.getline(line, 256) && lineCount < 100) {
        if (line[0] != '\0') {
            int i = 0;
            while (line[i] != '\0' && i < 255) {
                allLines[lineCount][i] = line[i];
                i++;
            }
            allLines[lineCount][i] = '\0';
            lineCount++;
        }
    }
    inFile.close();

    // Get timestamp
    time_t now = time(0);
    char dateBuf[32];
    ctime_s(dateBuf, sizeof(dateBuf), &now);
    int len = 0;
    while (dateBuf[len] != '\0' && dateBuf[len] != '\n' && len < 31) len++;
    dateBuf[len] = '\0';

    // Check if player exists
    bool found = false;
    for (int i = 0; i < lineCount; i++) {
        // Extract name from line
        char nameInFile[32];
        int n = 0;
        int pos = 0;
        while (allLines[i][pos] != '|' && allLines[i][pos] != '\0' && n < 31) {
            nameInFile[n++] = allLines[i][pos++];
        }
        nameInFile[n] = '\0';

        if (strcmp(nameInFile, playerName) == 0) {
            // Found — extract old score
            pos++; // skip |
            int oldScore = 0;
            while (allLines[i][pos] >= '0' && allLines[i][pos] <= '9') {
                oldScore = oldScore * 10 + (allLines[i][pos] - '0');
                pos++;
            }
            pos++; // skip |
            int oldLevel = 0;
            while (allLines[i][pos] >= '0' && allLines[i][pos] <= '9') {
                oldLevel = oldLevel * 10 + (allLines[i][pos] - '0');
                pos++;
            }

            // Only update if new score is higher
            if (score > oldScore) {
                std::string newLine = std::string(playerName) + "|" + std::to_string(score) + "|" + std::to_string(levelReached) + "|" + dateBuf;
                int pos = 0;
                while (newLine[pos] != '\0' && pos < 255) {
                    allLines[i][pos] = newLine[pos];
                    pos++;
                }
                allLines[i][pos] = '\0';
            }
            found = true;
            break;
        }
    }

    // Write back
    std::ofstream outFile("leaderboard.txt");
    if (!outFile.is_open()) return false;

    for (int i = 0; i < lineCount; i++) {
        outFile << allLines[i] << "\n";
    }

    if (!found) {
        outFile << playerName << "|" << score << "|" << levelReached << "|" << dateBuf << "\n";
    }

    outFile.close();

    std::cout << "Leaderboard saved: " << playerName << " score=" << score << std::endl;
    return true;
}

SaveManager::LeaderboardEntry** SaveManager::getTopScores(int count, int& outCount) {
    std::ifstream file("leaderboard.txt");
    if (!file.is_open()) {
        outCount = 0;
        return nullptr;
    }

    // Count entries
    int totalEntries = 0;
    char line[256];
    while (file.getline(line, 256)) totalEntries++;

    if (totalEntries == 0) {
        outCount = 0;
        file.close();
        return nullptr;
    }

    file.clear();
    file.seekg(0);

    // Read all entries
    LeaderboardEntry** entries = new LeaderboardEntry * [totalEntries];
    int idx = 0;
    while (file.getline(line, 256) && idx < totalEntries) {
        entries[idx] = new LeaderboardEntry();

        char* token = line;
        char* next = nullptr;

        // Name
        for (int i = 0; i < 31 && token[i] != '|' && token[i] != '\0'; i++) {
            entries[idx]->name[i] = token[i];
            entries[idx]->name[i + 1] = '\0';
        }
        while (*token != '|' && *token != '\0') token++;
        if (*token == '|') token++;

        // Score
        entries[idx]->score = 0;
        while (*token >= '0' && *token <= '9') {
            entries[idx]->score = entries[idx]->score * 10 + (*token - '0');
            token++;
        }
        while (*token != '|' && *token != '\0') token++;
        if (*token == '|') token++;

        // Level reached
        entries[idx]->levelReached = 0;
        while (*token >= '0' && *token <= '9') {
            entries[idx]->levelReached = entries[idx]->levelReached * 10 + (*token - '0');
            token++;
        }
        while (*token != '|' && *token != '\0') token++;
        if (*token == '|') token++;

        // Date
        int d = 0;
        while (*token != '\0' && d < 31) {
            entries[idx]->date[d++] = *token++;
        }
        entries[idx]->date[d] = '\0';

        idx++;
    }
    file.close();

    // Sort by score descending (simple bubble sort)
    for (int i = 0; i < idx - 1; i++) {
        for (int j = 0; j < idx - i - 1; j++) {
            if (entries[j]->score < entries[j + 1]->score) {
                LeaderboardEntry* temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }

    outCount = (idx < count) ? idx : count;
    return entries;
}