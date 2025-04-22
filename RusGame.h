#pragma once
#include "UI Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <string>
#include <functional>
#include <random>
#include "UI GameScreen.h"
#include "InputFieldHandler.h"
#include "ResultScreenRus.h"
class RusGame : public Screen {
public:
    RusGame(sf::Font& font, std::function<void(Screen*)> switchScreenCallback, std::string difficulty);
    ~RusGame();

    void draw(sf::RenderWindow& window) override;
    void handleInput(sf::Event& event, sf::RenderWindow& window) override;
    void update(float deltaTime) override;

private:
    void generateConstraint();
    void nextRound();
    void submitWord();
    bool isValidWord(const std::wstring& word); // Можно доработать под подключение словаря
    void endGame();

    sf::Font& font;
    std::function<void(Screen*)> switchScreenCallback;

    sf::Text constraintText;
    sf::Text scoreText;
    sf::Text timerText;
    sf::Text inputLabel;
    sf::Text wordInput;

    Button* submitButton;
    Button* skipButton;
    Button* surrenderButton;

    std::vector<std::wstring> usedWords;
    std::wstring currentInput;
    std::wstring requiredPart;
    std::wstring constraintType;
    std::string difficultyLevel;

    std::set<std::wstring> wordHistory;

    int currentRound;
    int maxRounds;
    int wordsPerRound;
    int scorePerWord;
    float roundTime;
    float timeLeft;

    int score;

    bool inputActive;

    std::mt19937 rng;

    sf::RectangleShape inputField;
    sf::Text inputText;
    bool isInputFieldSelected;
    bool cursorVisible;
    float cursorTimer;
    InputFieldHandler inputFieldHandler;
    int wordsEnteredThisRound;
};
