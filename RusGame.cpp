    #include "RusGame.h"
    #include <sstream>
    #include <iostream>
#include "UI MainMenuScreen.h"
    static const std::vector<std::wstring> prefixes = { L"пре", L"при", L"на", L"из" };
    static const std::vector<std::wstring> roots = { L"вод", L"гор", L"зар", L"зор" };
    static const std::vector<std::wstring> suffixes = { L"тель", L"чик", L"щик", L"к" };
    static const std::vector<std::wstring> endings = { L"ой", L"ий", L"а", L"я" };

    RusGame::RusGame(sf::Font& font, std::function<void(Screen*)> switchScreenCallback, std::string difficulty)
        : font(font), switchScreenCallback(switchScreenCallback), difficultyLevel(difficulty), rng(std::random_device{}()), inputFieldHandler(inputText, cursorVisible, cursorTimer)
    {
        if (difficulty == "easy") {
            roundTime = 30.f; maxRounds = 5; wordsPerRound = 1; scorePerWord = 5;
        }
        else if (difficulty == "medium") {
            roundTime = 25.f; maxRounds = 10; wordsPerRound = 2; scorePerWord = 10;
        }
        else {
            roundTime = 20.f; maxRounds = 15; wordsPerRound = 3; scorePerWord = 15;
        }

        currentRound = 0;
        score = 0;
        timeLeft = roundTime;
        inputActive = true;

        wordInput.setFont(font);
        wordInput.setCharacterSize(24);
        wordInput.setFillColor(sf::Color::Black);
        wordInput.setPosition(100, 300);

        inputLabel.setFont(font);
        inputLabel.setCharacterSize(18);
        inputLabel.setString(L"Введите слово:");
        inputLabel.setPosition(100, 260);
        inputLabel.setFillColor(sf::Color::Black);

        constraintText.setFont(font);
        constraintText.setCharacterSize(24);
        constraintText.setPosition(100, 50);
        constraintText.setFillColor(sf::Color::Blue);

        scoreText.setFont(font);
        scoreText.setCharacterSize(18);
        scoreText.setPosition(600, 50);
        scoreText.setFillColor(sf::Color::Black);

        timerText.setFont(font);
        timerText.setCharacterSize(18);
        timerText.setPosition(600, 80);
        timerText.setFillColor(sf::Color::Red);

        submitButton = new Button({ 100, 400 }, font, L"Подтвердить");
        skipButton = new Button({ 300, 400 }, font, L"Пропустить");
        surrenderButton = new Button({ 500, 400 }, font, L"Сдаться");

        inputField.setSize({ 400, 40 });
        inputField.setPosition(100, 300);
        inputField.setFillColor(sf::Color(240, 240, 240));
        inputField.setOutlineColor(sf::Color::Black);
        inputField.setOutlineThickness(2);

        inputText.setFont(font);
        inputText.setCharacterSize(24);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(110, 310);

        isInputFieldSelected = true;
        cursorVisible = true;
        cursorTimer = 0.0f;

        generateConstraint();
        nextRound();
    }

    RusGame::~RusGame() {
        delete submitButton;
        delete skipButton;
        delete surrenderButton;
    }

    void RusGame::generateConstraint() {
        std::vector<std::wstring> types = { L"Приставка", L"Корень", L"Суффикс", L"Окончание" };
        std::uniform_int_distribution<int> dist(0, 3);
        int typeIndex = dist(rng);
        constraintType = types[typeIndex];

        switch (typeIndex) {
        case 0: requiredPart = prefixes[rng() % prefixes.size()]; break;
        case 1: requiredPart = roots[rng() % roots.size()]; break;
        case 2: requiredPart = suffixes[rng() % suffixes.size()]; break;
        case 3: requiredPart = endings[rng() % endings.size()]; break;
        }

        constraintText.setString(L"Обязательная часть: " + constraintType + L" \"" + requiredPart + L"\"");
    }

    void RusGame::nextRound() {
        currentInput.clear();
        wordsEnteredThisRound = 0;
        currentRound++;
        timeLeft = roundTime;

        if (currentRound > maxRounds) {
            endGame();
            return;
        }
    }

    void RusGame::submitWord() {
        if (isValidWord(currentInput)) {
            score += scorePerWord;
            usedWords.push_back(currentInput);
            wordHistory.insert(currentInput);
            wordsEnteredThisRound++;
        }

        currentInput.clear();

        if (wordsEnteredThisRound >= wordsPerRound) {
            nextRound();
        }
    }


    bool RusGame::isValidWord(const std::wstring& word) {
        if (word.empty() || wordHistory.count(word) > 0) return false;
        return word.find(requiredPart) != std::wstring::npos;
    }

    void RusGame::endGame() {
        std::wcout << L"Игра окончена! Ваш счёт: " << score << std::endl;
        // Здесь можно передать управление экрану с рейтингом
        // или вернуться в главное меню
        if (switchScreenCallback) {
            switchScreenCallback(new ResultScreenRus(font, switchScreenCallback, score));
        }
    }

    void RusGame::handleInput(sf::Event& event, sf::RenderWindow& window) {
        if (isInputFieldSelected && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) {
                if (!currentInput.empty()) currentInput.pop_back();
            }
            else if (event.text.unicode >= 1040 && event.text.unicode <= 1103) {
                currentInput += static_cast<wchar_t>(event.text.unicode);
            }
            inputText.setString(currentInput);
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Return) {
                submitWord();
            }
            if (event.key.control && event.key.code == sf::Keyboard::V) {
                std::cout << "CTRL+V заблокировано\n";
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

            if (submitButton->isClicked(mousePos)) {
                submitWord();
            }
            else if (skipButton->isClicked(mousePos)) {
                nextRound(); // просто переход к следующему раунду
            }
            else if (surrenderButton->isClicked(mousePos)) {
                endGame(); // завершить игру досрочно
            }
        }

    }

    void RusGame::update(float deltaTime) {
        timeLeft -= deltaTime;
        if (timeLeft <= 0.f) {
            nextRound();
        }

        timerText.setString(L"Осталось времени: " + std::to_wstring(static_cast<int>(timeLeft)) + L" сек");
        scoreText.setString(L"Счёт: " + std::to_wstring(score));

        inputFieldHandler.updateCursor(deltaTime);
        inputText.setString(currentInput);
    }

    void RusGame::draw(sf::RenderWindow& window) {
        window.draw(inputField);
        window.draw(inputText);

        if (isInputFieldSelected && cursorVisible) {
            sf::RectangleShape cursor(sf::Vector2f(2, 24));
            cursor.setPosition(inputText.findCharacterPos(currentInput.size()).x, inputText.getPosition().y);
            cursor.setFillColor(sf::Color::Black);
            window.draw(cursor);
        }

        window.draw(constraintText);
        window.draw(inputLabel);
        window.draw(wordInput);
        window.draw(scoreText);
        window.draw(timerText);

        submitButton->draw(window);
        skipButton->draw(window);
        surrenderButton->draw(window);
    }
