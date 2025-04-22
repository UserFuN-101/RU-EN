#pragma once

#include "UI Button.h"
#include "UI Screen.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class DifficultyScreen_RusGame : public Screen {
public:
    DifficultyScreen_RusGame(sf::Font& font, std::function<void(Screen*)> switchScreenCallback);
    ~DifficultyScreen_RusGame();

    void draw(sf::RenderWindow& window) override;
    void handleInput(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;

private:
    sf::Font& font;
    std::function<void(Screen*)> switchScreenCallback;

    Button* easyButton;
    Button* mediumButton;
    Button* hardButton;
    Button* backButton;
};
