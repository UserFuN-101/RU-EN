#include "DifficultyScreen_RusGame.h"
#include "UI MainMenuScreen.h"
#include "RusGame.h" // Подключаем главный игровой класс
#include <iostream>

DifficultyScreen_RusGame::DifficultyScreen_RusGame(sf::Font& font, std::function<void(Screen*)> switchScreenCallback)
    : font(font), switchScreenCallback(switchScreenCallback)
{
    easyButton = new Button({ 100, 100 }, font, L"Легко");
    mediumButton = new Button({ 100, 170 }, font, L"Средне");
    hardButton = new Button({ 100, 240 }, font, L"Сложно");
    backButton = new Button({ 100, 310 }, font, L"Назад");
}

DifficultyScreen_RusGame::~DifficultyScreen_RusGame() {
    delete easyButton;
    delete mediumButton;
    delete hardButton;
    delete backButton;
}

void DifficultyScreen_RusGame::draw(sf::RenderWindow& window) {
    easyButton->draw(window);
    mediumButton->draw(window);
    hardButton->draw(window);
    backButton->draw(window);
}

void DifficultyScreen_RusGame::handleInput(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (easyButton->isClicked(mousePos)) {
            std::cout << "Выбрана сложность: easy" << std::endl;
            if (switchScreenCallback) {
                switchScreenCallback(new RusGame(font, switchScreenCallback, "easy"));
            }
        }
        else if (mediumButton->isClicked(mousePos)) {
            std::cout << "Выбрана сложность: medium" << std::endl;
            if (switchScreenCallback) {
                switchScreenCallback(new RusGame(font, switchScreenCallback, "medium"));
            }
        }
        else if (hardButton->isClicked(mousePos)) {
            std::cout << "Выбрана сложность: hard" << std::endl;
            if (switchScreenCallback) {
                switchScreenCallback(new RusGame(font, switchScreenCallback, "hard"));
            }
        }
        else if (backButton->isClicked(mousePos)) {
            std::cout << "Назад в главное меню" << std::endl;
            if (switchScreenCallback) {
                switchScreenCallback(new MainMenuScreen(font, switchScreenCallback));
            }
        }
    }
}

void DifficultyScreen_RusGame::update(float deltaTime) {
    // Пока ничего не нужно обновлять
}
