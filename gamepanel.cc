#include "gamepanel.h"
#include "constants.h"
#include <iostream>
#include <fstream>
#include <filesystem>

void GamePanel::centerOrigin(sf::Text& text) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}

GamePanel::GamePanel() {
    font.loadFromFile("/System/Library/Fonts/Courier.ttc");
    text_asteroids.setFont(font);
    text_asteroids.setCharacterSize(100);
    text_asteroids.setString("A S T E R O I D S");
    centerOrigin(text_asteroids);
    text_asteroids.setPosition(constants::window_width / 2, 100);
    text_asteroids.setFillColor(sf::Color::White);

    text_paused.setFont(font);
    text_paused.setCharacterSize(100);
    text_paused.setString("P A U S E D");
    centerOrigin(text_paused);
    text_paused.setPosition(constants::window_width / 2, constants::window_height / 2);
    text_paused.setFillColor(sf::Color::White);

    text_score.setFont(font);
    text_score.setCharacterSize(24);
    text_score.setString("Score: 0");
    centerOrigin(text_score);
    text_score.setPosition(100, 30);
    text_score.setFillColor(sf::Color::White);

    text_lives.setFont(font);
    text_lives.setCharacterSize(24);
    text_lives.setString("Lives: " + std::to_string(constants::player_lives));
    centerOrigin(text_lives);
    text_lives.setPosition(constants::window_width - 100, 30);
    text_lives.setFillColor(sf::Color::Green);

    text_level.setFont(font);
    text_level.setCharacterSize(24);
    text_level.setString("Level: 1");
    centerOrigin(text_level);
    text_level.setPosition(constants::window_width / 2, 30);
    text_level.setFillColor(sf::Color::White);

    text_start.setFont(font);
    text_start.setCharacterSize(50);
    text_start.setString("Press ENTER to Start");
    centerOrigin(text_start);
    text_start.setPosition(constants::window_width / 2, constants::window_height / 2);
    text_start.setFillColor(sf::Color::White);

    text_hall_of_fame.setFont(font);
    text_hall_of_fame.setCharacterSize(50);
    text_hall_of_fame.setString("Press H to Hall of Fame");
    centerOrigin(text_hall_of_fame);
    text_hall_of_fame.setPosition(constants::window_width / 2, constants::window_height / 2 + 100);
    text_hall_of_fame.setFillColor(sf::Color::White);

    text_game_over.setFont(font);
    text_game_over.setCharacterSize(60);
    text_game_over.setString("Game Over!");
    centerOrigin(text_game_over);
    text_game_over.setPosition(constants::window_width / 2, constants::window_height / 2 - 50);
    text_game_over.setFillColor(sf::Color::Red);

    text_high_scores.setFont(font);
    text_high_scores.setCharacterSize(35);
    text_high_scores.setString("H i g h  S c o r e s");
    centerOrigin(text_high_scores);
    text_high_scores.setPosition(constants::window_width / 2, 100);
    text_high_scores.setFillColor(sf::Color::White);

    text_back.setFont(font);
    text_back.setCharacterSize(30);
    text_back.setString("Press B to back");
    centerOrigin(text_back);
    text_back.setPosition(constants::window_width / 2, constants::window_height - 100);
    text_back.setFillColor(sf::Color::White);
}

void GamePanel::updateScore(int score) {
    text_score.setString("Score: " + std::to_string(score));
}

void GamePanel::updateLives(int lives) {
    text_lives.setString("Lives: " + std::to_string(lives));
}

void GamePanel::updateLevel(int level) {
    text_level.setString("Level: " + std::to_string(level));
}

void GamePanel::saveHighScores() {
    std::ofstream file{"high_scores.txt"};
    for (const auto& p : high_scores_list) {
        file << p.second << std::endl;
    }
    std::cout << "Saving Score..." << std::endl;
    file.close();
}

void GamePanel::loadHighScores() {
    std::ifstream file{"high_scores.txt"};
    std::string line;
    high_scores_list.clear();
    while (getline(file, line)) {
        sf::Text scoreText;
        scoreText.setString(line);
        high_scores_list.push_back({scoreText, stod(line)});
    }
    file.close();
}

void GamePanel::updateHighScores(long score) {
    sf::Text scoreText;
    scoreText.setString(std::to_string(score));
    high_scores_list.push_back({scoreText, score});
    std::sort(high_scores_list.begin(), high_scores_list.end(), [](const std::pair<sf::Text, long> a, const std::pair<sf::Text, long> b) {
        return a.second > b.second;
    });
    if (high_scores_list.size() > constants::HIGH_SCORE_NUMS) {
        high_scores_list.erase(high_scores_list.begin() + constants::HIGH_SCORE_NUMS);
    }
}

void GamePanel::draw(sf::RenderWindow& window) {
    switch (currentState) {
        case PanelState::StartScreen:
            window.draw(text_start);
            window.draw(text_asteroids);
            window.draw(text_hall_of_fame);
            break;
        case PanelState::GamePlay:
//            std::cout << "Playing!" << std::endl;
            window.draw(text_score);
            window.draw(text_lives);
            window.draw(text_level);
            break;
        case PanelState::GamePaused:
//            std::cout << "Paused!" << std::endl;
            window.draw(text_score);
            window.draw(text_lives);
            window.draw(text_paused);
            window.draw(text_level);
            break;
        case PanelState::GameOver:
//            std::cout << "Game Over!" << std::endl;
            window.draw(text_game_over);
            break;
        case PanelState::HighScores:
            window.draw(text_high_scores);
            window.draw(text_back);
            float y = 150;
            for (auto& p : high_scores_list) {
                auto scoreText = p.first;
                centerOrigin(scoreText);
                scoreText.setPosition(constants::window_width / 2.0f - 20, y);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setFont(font);
                scoreText.setCharacterSize(24);
                y += 30;
                window.draw(scoreText);
            }
            break;
    }
}

void GamePanel::setState(PanelState newState) {
    currentState = newState;
}
