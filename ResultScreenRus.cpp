#include "ResultScreenRus.h"
#include "RusGame.h"
#include "UI MainMenuScreen.h" // <-- ���� � ���� ���� ������� �����

ResultScreenRus::ResultScreenRus(sf::Font& font, std::function<void(Screen*)> switchScreenCallback, int finalScore)
    : font(font), switchScreenCallback(switchScreenCallback), finalScore(finalScore)
{
    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setString(L"���� ��������!");
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(300, 100);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setString(L"��� ����: " + std::to_wstring(finalScore));
    scoreText.setFillColor(sf::Color::Blue);
    scoreText.setPosition(300, 160);

    playAgainButton = new Button({ 300, 250 }, font, L"������� �����");
    mainMenuButton = new Button({ 300, 320 }, font, L"� ������� ����");
}

ResultScreenRus::~ResultScreenRus() {
    delete playAgainButton;
    delete mainMenuButton;
}

void ResultScreenRus::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(scoreText);
    playAgainButton->draw(window);
    mainMenuButton->draw(window);
}

void ResultScreenRus::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

        if (playAgainButton->isClicked(mousePos)) {
            switchScreenCallback(new RusGame(font, switchScreenCallback, "easy")); // ��� ������ ������� ���������
        }
        else if (mainMenuButton->isClicked(mousePos)) {
            switchScreenCallback(new MainMenuScreen(font, switchScreenCallback)); // ���� � ���� ���� ����
        }
    }
}

void ResultScreenRus::update(float deltaTime) {
    // ����� �������� �������� �����
}
