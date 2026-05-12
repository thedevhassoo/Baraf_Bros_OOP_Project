#include "LeaderboardDialog.h"
#include "SceneManager.h"
#include <fstream>

LeaderboardDialog::LeaderboardDialog()
    : leaderboardItems(nullptr), itemCount(0),
    closeButton("./assets/leaderboard/close_btn0001.png",
        "./assets/leaderboard/close_btn0002.png",
        "./assets/leaderboard/close_btn0003.png", this, "closeButton") {

    float startX = 367.1f, startY = 87.0f;
    float height = 55.0f;

    dialogTexture.loadFromFile("./assets/leaderboard/leaderboard.png");
    dialogSprite.setTexture(dialogTexture);
    closeButton.setPos({ 866.85f, 39.7f });
    dialogSprite.setPosition({
        (1280.0f / 2.0f) - (dialogTexture.getSize().x / 2.0f),
        (720.0f / 2.0f) - (dialogTexture.getSize().y / 2.0f)
        });

    loadLeaderboard();
}

LeaderboardDialog::~LeaderboardDialog()
{
	for (int i = 0; i < itemCount; ++i) {
        delete leaderboardItems[i];
    }
    delete[] leaderboardItems;
}

void LeaderboardDialog::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    closeButton.handleEvent(event, window);
	// Handle other events if necessary (e.g., clicking on leaderboard items)
}

void LeaderboardDialog::update(float dt)
{
}

void LeaderboardDialog::draw(sf::RenderWindow& window)
{
    window.draw(blockTexture);
    window.draw(dialogSprite);
    closeButton.draw(window);
    for (int i = 0; i < itemCount; ++i) {     
        leaderboardItems[i]->draw(window);
    }
}

void LeaderboardDialog::onButtonClick(const std::string& buttonId)
{
	if (buttonId == "closeButton") {
        sceneManager->closeDialog();
    }
}

void LeaderboardDialog::loadLeaderboard() {
    // Free old items
    if (leaderboardItems) {
        for (int i = 0; i < itemCount; ++i) delete leaderboardItems[i];
        delete[] leaderboardItems;
        leaderboardItems = nullptr;
    }
    itemCount = 0;

    std::ifstream file("leaderboard.txt");
    if (!file.is_open()) {
        // No file yet — show empty
        leaderboardItems = nullptr;
        return;
    }

    // Count lines
    int totalLines = 0;
    char line[256];
    while (file.getline(line, 256)) {
        if (line[0] != '\0') totalLines++;
    }

    if (totalLines == 0) {
        file.close();
        return;
    }

    // Read all entries
    struct Entry {
        char name[32];
        int score;
        int level;
        char date[32];
    };

    Entry* allEntries = new Entry[totalLines];
    file.clear();
    file.seekg(0);

    int idx = 0;
    while (file.getline(line, 256) && idx < totalLines) {
        char* p = line;

        // Name
        int n = 0;
        while (*p != '|' && *p != '\0' && n < 31) {
            allEntries[idx].name[n++] = *p++;
        }
        allEntries[idx].name[n] = '\0';
        if (*p == '|') p++;

        // Score
        allEntries[idx].score = 0;
        while (*p >= '0' && *p <= '9') {
            allEntries[idx].score = allEntries[idx].score * 10 + (*p - '0');
            p++;
        }
        if (*p == '|') p++;

        // Level
        allEntries[idx].level = 0;
        while (*p >= '0' && *p <= '9') {
            allEntries[idx].level = allEntries[idx].level * 10 + (*p - '0');
            p++;
        }
        if (*p == '|') p++;

        // Date
        int d = 0;
        while (*p != '\0' && d < 31) {
            allEntries[idx].date[d++] = *p++;
        }
        allEntries[idx].date[d] = '\0';

        idx++;
    }
    file.close();

    // Sort by score descending
    for (int i = 0; i < totalLines - 1; i++) {
        for (int j = 0; j < totalLines - i - 1; j++) {
            if (allEntries[j].score < allEntries[j + 1].score) {
                Entry temp = allEntries[j];
                allEntries[j] = allEntries[j + 1];
                allEntries[j + 1] = temp;
            }
        }
    }

    // Show top 10 or fewer
    itemCount = (totalLines > 10) ? 10 : totalLines;
    leaderboardItems = new LeaderboardItem * [itemCount];

    float startX = 367.1f;
    float startY = 87.0f;
    float height = 55.0f;

    for (int i = 0; i < itemCount; i++) {
        std::string scoreStr = std::to_string(allEntries[i].score);
        leaderboardItems[i] = new LeaderboardItem(i + 1, allEntries[i].name, scoreStr);
        leaderboardItems[i]->setPos({ startX, startY + (height * i) });
    }

    delete[] allEntries;
}
