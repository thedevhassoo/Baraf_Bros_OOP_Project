#pragma once 
#include <SFML/Graphics.hpp>
#include <string>

class TextBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Text placeholder;
    sf::Font font;

    sf::String input;
    bool isSelected;
    bool hasLimit;
    int charLimit;

    // Colors
    sf::Color bgColorNormal;
    sf::Color bgColorSelected;
    sf::Color textColor;
    sf::Color placeholderColor;
    sf::Color outlineColor;

    // Outline
    float outlineThickness;
    bool hasOutline;

    // Cursor
    bool showCursor;
    float cursorTimer;
    float cursorBlinkSpeed;

    std::string placeholderText;
    bool hidden;

public:
    TextBox();
    ~TextBox();

    // Initialization
    bool init(float x, float y, float width, float height,
        sf::Font fontPath, const std::string& placeholderStr = "");

    void setHidden(bool hide);

    // Event handling
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    // Color setters
    void setBackgroundColor(sf::Color color);
    void setSelectedBackgroundColor(sf::Color color);
    void setTextColor(sf::Color color);
    void setPlaceholderColor(sf::Color color);
    void setOutlineColor(sf::Color color);

    // Outline setters
    void setOutlineThickness(float thickness);
    void enableOutline();
    void disableOutline();

    // Position and size
    void setPosition(float x, float y);
    void setSize(float width, float height);

    // Text settings
    void setCharacterLimit(int limit);
    void removeLimit();
    void setFontSize(unsigned int size);
    void setCursorBlinkSpeed(float seconds);
    void setPlaceholder(const std::string& text);

    // State
    void clear();
    void setSelected(bool selected);

    // Getters
    sf::String getString() const;
    std::string getStringAsString() const;
    bool isSelectedBox() const;
    float getWidth() const;
    float getHeight() const;
    sf::Vector2f getPosition() const;

private:
    void updateDisplay();
    void applyOutline();
};
