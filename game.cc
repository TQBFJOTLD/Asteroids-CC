#include <algorithm>
#include <cmath>
#include "game.h"
#include "player.h"
#include "asteroid.h"
#include <iostream>

Game::Game() {
    game_window.setFramerateLimit(60);
}

bool entity_manager::refresh(long &score) {
    std::vector<std::unique_ptr<Entity>> newEntities;
    for (auto& e : all_entities) {
        auto generated = e->onDestroy(score);
        for (auto& g : generated) {
            auto ptr_alias = g.get();
            auto hash = typeid(*g).hash_code();
            grouped_entities[hash].emplace_back(ptr_alias);

            newEntities.push_back(std::move(g));
        }
    }
    std::move(newEntities.begin(), newEntities.end(), std::back_inserter(all_entities));

    for (auto& [type, alias_vector] : grouped_entities) {
        alias_vector.erase(remove_if(begin(alias_vector), end(alias_vector),
                                     [](const auto& p) { return p->is_destroyed(); }
                           ),
                           end(alias_vector));
    }

    all_entities.erase(remove_if(begin(all_entities), end(all_entities),
                                 [](const auto& p) { return p->is_destroyed(); }
                       ),
                       end(all_entities));

    return grouped_entities[Asteroid::typeHash].empty();;
}

// Function to destroy all entities
void entity_manager::clear() {
    grouped_entities.clear();
    all_entities.clear();
}

// Function to update all the entities
void entity_manager::update(bool paused) {
    if (paused) return;
    for (auto& e : all_entities)
        e->update();
}

// Function to update make all the entities draw themselves
void entity_manager::draw(sf::RenderWindow& window, bool paused) {
    if (paused) return;
    for (auto& e : all_entities)
        e->draw(window);
}

void Game::reset() {
    panel.currentState = PanelState::StartScreen;
    Player::lives = constants::player_lives;
//    manager.clear();
    level = 1;
    score = 0;
    alive = true;
    lastGameOverTime = std::numeric_limits<float>::max();
    lastBulletTime = 0;

    game_window.setFramerateLimit(60);
    game_window.setKeyRepeatEnabled(false);
}

void Game::generateAsteroids(Player& player, int level) {
    if (Player::lives == 0) return;
    for (int i = 0; i < constants::asteroids_num + level; ++i) {
        bool collision = false;
        Asteroid* game_asteroid = nullptr;
        do {
            if (game_asteroid) {
                game_asteroid->destroy();
            }
            float asteroidX = static_cast<float>(rand() % static_cast<int>(constants::window_width));
            float asteroidY = static_cast<float>(rand() % static_cast<int>(constants::window_height));

            game_asteroid = &manager.create<Asteroid>(asteroidX, asteroidY, constants::LARGE_RATIO);
            collision = game_asteroid->intersects(player);

        } while (collision);
    }
}

void Game::run() {
    game_window.setKeyRepeatEnabled(false);
    panel.loadHighScores();

    auto& game_player = manager.create<Player>(constants::window_width/2.0f, constants::window_height/2.0f);
    generateAsteroids(game_player, level);
    paused = true;

    while (game_window.isOpen()) {
        game_window.clear(sf::Color::Black);
        sf::Event event;

        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                game_window.close();
        }

        float currentTime = clock.getElapsedTime().asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            if (currentTime - lastBulletTime >= constants::bullets_cooldown) {
                manager.create<Bullet>(game_player.getX(), game_player.getY(), 2.0f, game_player.getAngle(), game_player.getSpeed() * 10);
                lastBulletTime = currentTime;
            }
        }

        manager.check_collision<Asteroid, Player>();
        manager.check_collision<Asteroid, Bullet>();

        manager.update(paused);
        if (manager.refresh(score)) {
            level ++;
            generateAsteroids(game_player, level);
        }

        game_window.clear(sf::Color::Black);
        panel.draw(game_window);
        panel.updateLives(game_player.lives);
        panel.updateScore(score);
        panel.updateLevel(level);
        if (panel.currentState != PanelState::StartScreen && panel.currentState != PanelState::HighScores) {
            manager.draw(game_window, paused);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) && panel.currentState == PanelState::StartScreen) {
            panel.currentState = PanelState::HighScores;
        }

        if (Player::lives == 0 && alive) {
            alive = false;
            panel.updateHighScores(score);
            panel.currentState = PanelState::GameOver;
            lastGameOverTime = currentTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && panel.currentState == PanelState::StartScreen) {
            panel.currentState = PanelState::GamePlay;
            paused = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && panel.currentState == PanelState::GamePlay) {
            reset();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B) && panel.currentState == PanelState::HighScores) {
            panel.currentState = PanelState::StartScreen;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
            paused = !paused;
            if (panel.currentState == PanelState::GamePlay) panel.currentState = PanelState::GamePaused;
            else if (panel.currentState == PanelState::GamePaused) panel.currentState = PanelState::GamePlay;
        }

        if (panel.currentState == PanelState::GameOver) {
            if (currentTime - lastGameOverTime > 2) {
                panel.saveHighScores();
                reset();
            }
        }

        game_window.display();
    }
}