#ifndef ASTEROIDS_GAMEPANEL_H
#define ASTEROIDS_GAMEPANEL_H

#include "SFML/Graphics.hpp"
#include <vector>
#include <utility>

enum class PanelState {
    StartScreen,
    GamePlay,
    GameOver,
    GamePaused,
    HighScores
};

class GamePanel {
    sf::Font font;
    sf::Text text_score;
    sf::Text text_asteroids;
    sf::Text text_lives;
    sf::Text text_start;
    sf::Text text_game_over;
    sf::Text text_game_restart;
    sf::Text text_back;
    sf::Text text_high_scores;
    sf::Text text_hall_of_fame;
    sf::Text text_paused;
    sf::Text text_level;
    std::vector<std::pair<sf::Text, long>> high_scores_list;

public:
    GamePanel();
    void updateScore(int score);
    void updateLives(int lives);
    void updateLevel(int level);
    void draw(sf::RenderWindow& window);
    void setState(PanelState newState);
    void updateHighScores(long score);
    void centerOrigin(sf::Text& text);
    void saveHighScores();
    void loadHighScores();
    PanelState currentState = PanelState::StartScreen;
};

#endif //ASTEROIDS_GAMEPANEL_H