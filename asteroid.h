#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "entity.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class Asteroid : public Entity {
    std::mt19937 gen;
public:
    Asteroid(double x, double y, double ratio);
    static const size_t typeHash;

//    sf::ConvexShape* generateAsteroid(double ratio);
    std::unique_ptr<sf::Shape> generateAsteroid(double ratio);
    std::vector<std::unique_ptr<Asteroid>> destroyed(double ratio, double destroyedSpeed);

    void update() override;
    void draw(sf::RenderWindow& window) override;
    std::vector<std::unique_ptr<Entity>> onDestroy(long &score) override;
};

#endif //ASTEROIDS_ASTEROID_H
