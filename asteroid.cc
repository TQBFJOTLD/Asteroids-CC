#include "asteroid.h"
#include <random>
#include "constants.h"
#include <iostream>

static const std::vector<std::vector<float>> points = {
        {0, -10, 10, -20, 20, -10, 17, 0, 20, 10, 10, 20, -10, 20, -20, 10, -20, -10, -20, -10, -10, -20},
        {0, -15, 10, -20, 20, -15, 10, -7, 20, 10, 12, 20, -6, 17, -10, 20, -20, 10, -15, 0, -20, -10, -20},
        {-10, -20, 10, -20, 20, -5, 20, 5, 10, 20, 0, 7, -5, 20, -20, 5, -10, 0, -20, -20, -5},
        {0, -15, -5, -20, 10, -20, 20, -10, 20, -5, 10, 0, 20, 5, 15, 20, 10, 15, -10, 20, -20, 5, -20, -15}
};

const size_t Asteroid::typeHash = typeid(Asteroid).hash_code();

Asteroid::Asteroid(double x, double y, double ratio) : Entity() {
    this->x = x;
    this->y = y;
    this->ratio = ratio;

    // Set the velocity based on the size
    speed = (ratio == constants::LARGE_RATIO) ? 1 : (ratio == constants::MEDIUM_RATIO) ? 2 : 3;
    dx = speed * std::sin(angle * M_PI / 180.0);
    dy = -speed * std::cos(angle * M_PI / 180.0);

    shape = generateAsteroid(ratio);
    shape->setPosition(x, y);
    shape->setFillColor(sf::Color::Black);
    shape->setOutlineThickness(3);
    shape->setOutlineColor(sf::Color::White);
}


std::unique_ptr<sf::Shape> Asteroid::generateAsteroid(double ratio) {
    auto convex = std::make_unique<sf::ConvexShape>();
    std::uniform_int_distribution<> dist(0, points.size() - 1);

    int index = dist(gen);
    const auto& chosenPoints = points[index];

    convex->setPointCount(chosenPoints.size() / 2);

    // Set points for the convex, applying the ratio to scale the coordinates
    for (int i = 0; i < chosenPoints.size(); i += 2) {
        convex->setPoint(i / 2, sf::Vector2f(chosenPoints[i] * ratio, chosenPoints[i + 1] * ratio));
    }

    return convex;
}

void Asteroid::update() {
    x += dx, y += dy;
    if (x < 0) {
        x = constants::window_width;
    } else if (x > constants::window_width) {
        x = 0;
    }
    if (y < 0) {
        y = constants::window_height;
    } else if (y > constants::window_height) {
        y = 0;
    }

    if (shape) shape->setPosition(x, y);
}

std::vector<std::unique_ptr<Entity>> Asteroid::onDestroy(long& score) {
    std::vector<std::unique_ptr<Entity>> newAsteroids;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> uni(0.0, 360.0);

    if (this->is_destroyed()) {
        if (this->ratio == constants::LARGE_RATIO) score += constants::LARGE_SCORE;
        else if (this->ratio == constants::MEDIUM_RATIO) score += constants::MEDIUM_SCORE;
        else score += constants::SMALL_SCORE;

        float newRatio = (this->ratio == constants::LARGE_RATIO) ? constants::MEDIUM_RATIO :
                          (this->ratio == constants::MEDIUM_RATIO) ? constants::SMALL_RATIO : 0;
        if (newRatio == 0) return {};

        for (int i = 0; i < constants::new_asteroids; ++i) {
            newAsteroids.push_back(std::make_unique<Asteroid>(x, y, newRatio));
            float new_angle = uni(rng);
            float new_speed = speed + static_cast<float>(rand()) / RAND_MAX * 0.5f;
            float new_dx = new_speed * std::sin(new_angle * M_PI / 180.0);
            float new_dy = -new_speed * std::cos(new_angle * M_PI / 180.0);

            newAsteroids.back()->setDx(new_dx);
            newAsteroids.back()->setDy(new_dy);
        }
    }

    return newAsteroids;
}

void Asteroid::draw(sf::RenderWindow& window) {
    if (shape) window.draw(*shape);
}
