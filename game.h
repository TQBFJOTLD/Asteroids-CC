#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include "SFML/Graphics.hpp"
#include "constants.h"
#include "entity.h"
#include "gamepanel.h"
#include "player.h"
#include <limits>
#include <vector>
#include <iostream>

// Use aliases to simplify the code
using entity_vector = std::vector<std::unique_ptr<Entity>>;
using entity_alias_vector = std::vector<Entity *>;

class entity_manager {
    entity_vector all_entities;
    std::map<size_t, entity_alias_vector> grouped_entities;
public:
    template <typename T, typename... Args>
    T& create(Args&&... args) {
        static_assert(std::is_base_of<Entity, T>::value,
                      R"("T" type parameter in create() must be derived from "entity")");

        auto ptr{std::make_unique<T>(std::forward<Args>(args)...)};
        auto ptr_alias = ptr.get();
        auto hash = typeid(T).hash_code();

        grouped_entities[hash].emplace_back(ptr_alias);
        all_entities.emplace_back(std::move(ptr));
        return *ptr_alias;
    }

    template <typename T1, typename T2>
    void check_collision() {
        auto& group1 = grouped_entities[typeid(T1).hash_code()];
        auto& group2 = grouped_entities[typeid(T2).hash_code()];
        if (typeid(T1) != typeid(T2)) {
            for (auto& entity1: group1) {
                for (auto& entity2: group2) {
                    if (entity1->intersects(*entity2)) {
                        entity1->destroy();
                        entity2->destroy();
                    }
                }
            }
        }
    }

    // Function to scan all entities and clean up the destroyed ones
    bool refresh(long &score);

    // Function to destroy all entities
    void clear();

    // Function to retrieve all the objects of a given type
    template <typename T>
    auto& get_all() {
        return grouped_entities[typeid(T).hash_code()];
    }

    // Apply a function to all entities of a given type
    template <typename T, typename Func>
    void apply_all(const Func& func) {
        auto& entity_group{get_all<T>()};

        for (auto ptr : entity_group)
            func(*dynamic_cast<T*>(ptr));
    }

    void update(bool paused);
    void draw(sf::RenderWindow& window, bool paused);
};

class Game {
    sf::RenderWindow game_window{{constants::window_width, constants::window_height}, "Asteroids"};
    entity_manager manager;

    sf::Font courier;
    sf::Text text_lives;
    sf::Clock clock;
    float lastBulletTime = 0;
    float lastGameOverTime = std::numeric_limits<float>::max();
    int level = 1;
    GamePanel panel;
    bool paused{false};
    long score = 0;
    bool alive = true;

public:
    Game();
    void reset();
    void run();
    void generateAsteroids(Player& player, int level);
};

#endif //ASTEROIDS_GAME_H
