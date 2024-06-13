#ifndef ASTEROIDS_BULLET_H
#define ASTEROIDS_BULLET_H

#include "entity.h"
#include <SFML/Graphics.hpp>

class Bullet : public Entity {
    sf::Clock lifetimeTimer;
public:
    Bullet(float x, float y, float radius, float angle, float speed);

    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif //ASTEROIDS_BULLET_H
