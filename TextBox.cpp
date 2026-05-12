// TextBox.cpp
#include "TextBox.h"

TextBox::TextBox()
    : isSelected(false)
    , hasLimit(false)
    , charLimit(0)
    , bgColorNormal(sf::Color::White)
    , bgColorSelected(sf::Color::White)
    , textColor(sf::Color::Black)
    , placeholderColor(sf::Color(150, 150, 150))
    , outlineColor(sf::Color::Black)
    , outlineThickness(0)
    , hasOutline(false)
    , showCursor(true)
    , cursorTimer(0)
    , cursorBlinkSpeed(0.5f)
    ,hidden(false){
}

TextBox::~TextBox() {
    // Nothing to clean up
}

bool TextBox::init(float x, float y, float width, float height,
    sf::Font f, const std::string& placeholderStr) {
    this->font = f;

    // Setup textbox background
    box.setSize(sf::Vector2f(width, height));
    box.setPosition(x, y);
    box.setFillColor(bgColorNormal);

    // Apply outline if enabled
    if (hasOutline) {
        box.setOutlineThickness(outlineThickness);
        box.setOutlineColor(outlineColor);
    }
    else {
        box.setOutlineThickness(0);
    }

    // Setup input text
    text.setFont(font);
    text.setCharacterSize(height - 10);
    text.setFillColor(textColor);
    text.setPosition(x, y);

    // Setup placeholder text
    placeholder.setFont(font);
    placeholder.setCharacterSize(height - 10);
    placeholder.setFillColor(placeholderColor);
    placeholder.setPosition(x, y);
    placeholder.setString(placeholderStr);
    placeholderText = placeholderStr;

    return true;
}
void TextBox::setHidden(bool hide) {
    hidden = hide;
}

void TextBox::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Check if clicked
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (box.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isSelected = true;
                box.setFillColor(bgColorSelected);
                cursorTimer = 0;
                showCursor = true;
                updateDisplay();
            }
            else {
                isSelected = false;
                box.setFillColor(bgColorNormal);
                updateDisplay();
            }
        }
    

    // Handle text input if selected
    if (isSelected && event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b') { // Backspace
                if (!input.isEmpty()) {
                    input.erase(input.getSize() - 1);
                }
            }
            else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                // Enter pressed - keep selected
            }
            else {
                if (!hasLimit || input.getSize() < charLimit) {
                  
                        input += event.text.unicode;
                    
                }
            }
            updateDisplay();
        }
    }
}

void TextBox::update(float dt) {
    if (isSelected) {
        cursorTimer += dt;
        if (cursorTimer >= cursorBlinkSpeed) {
            cursorTimer = 0;
            showCursor = !showCursor;
            updateDisplay();
        }
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);

    if (input.isEmpty() && placeholder.getString() != "") {
        window.draw(placeholder);
    }
    else {
        window.draw(text);
    }
}

void TextBox::updateDisplay() {
    if (input.isEmpty()) {
        text.setString("");
        if (isSelected && showCursor) {
            placeholder.setString("|");
        }
        else {
            placeholder.setString(placeholderText);
        }
    }
    else {
        std::string hiddenText = "";
		while (input.getSize() > hiddenText.size()) {
            hiddenText += "*";
        }
        if (isSelected && showCursor) {
            if(hidden) 
                text.setString(hiddenText + "|");
            
			else
            text.setString(input + "|");
        }
        else {
            if (hidden) 
                text.setString(hiddenText);
            
			else
            text.setString(input);
        }
        placeholder.setString("");
    }
}

void TextBox::applyOutline() {
    if (hasOutline) {
        box.setOutlineThickness(outlineThickness);
        box.setOutlineColor(outlineColor);
    }
    else {
        box.setOutlineThickness(0);
    }
}

// Color Setters
void TextBox::setBackgroundColor(sf::Color color) {
    bgColorNormal = color;
    if (!isSelected) {
        box.setFillColor(bgColorNormal);
    }
}

void TextBox::setSelectedBackgroundColor(sf::Color color) {
    bgColorSelected = color;
    if (isSelected) {
        box.setFillColor(bgColorSelected);
    }
}

void TextBox::setTextColor(sf::Color color) {
    textColor = color;
    text.setFillColor(textColor);
}

void TextBox::setPlaceholderColor(sf::Color color) {
    placeholderColor = color;
    placeholder.setFillColor(placeholderColor);
}

void TextBox::setOutlineColor(sf::Color color) {
    outlineColor = color;
    if (hasOutline) {
        box.setOutlineColor(outlineColor);
    }
}

// Outline Setters
void TextBox::setOutlineThickness(float thickness) {
    outlineThickness = thickness;
    applyOutline();
}

void TextBox::enableOutline() {
    hasOutline = true;
    applyOutline();
}

void TextBox::disableOutline() {
    hasOutline = false;
    applyOutline();
}

// Position and Size
void TextBox::setPosition(float x, float y) {
    box.setPosition(x, y);
    text.setPosition(x, y);
    placeholder.setPosition(x, y);
}

void TextBox::setSize(float width, float height) {
    box.setSize(sf::Vector2f(width, height));
    text.setCharacterSize(height - 10);
    placeholder.setCharacterSize(height - 10);
}

// Text Settings
void TextBox::setCharacterLimit(int limit) {
    hasLimit = true;
    charLimit = limit;
}

void TextBox::removeLimit() {
    hasLimit = false;
}

void TextBox::setFontSize(unsigned int size) {
    text.setCharacterSize(size);
    placeholder.setCharacterSize(size);
}

void TextBox::setCursorBlinkSpeed(float seconds) {
    cursorBlinkSpeed = seconds;
}

void TextBox::setPlaceholder(const std::string& text) {
    placeholderText = text;
    placeholder.setString(text);
    updateDisplay();
}

// State
void TextBox::clear() {
    input.clear();
    updateDisplay();
}

void TextBox::setSelected(bool selected) {
    isSelected = selected;
    if (isSelected) {
        box.setFillColor(bgColorSelected);
    }
    else {
        box.setFillColor(bgColorNormal);
    }
    updateDisplay();
}

// Getters
sf::String TextBox::getString() const {
    return input;
}

std::string TextBox::getStringAsString() const {
    return input.toAnsiString();
}

bool TextBox::isSelectedBox() const {
    return isSelected;
}

float TextBox::getWidth() const {
    return box.getSize().x;
}

float TextBox::getHeight() const {
    return box.getSize().y;
}

sf::Vector2f TextBox::getPosition() const {
    return box.getPosition();
}