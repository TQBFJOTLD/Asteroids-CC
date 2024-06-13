#ifndef ASTEROIDS_ENTITY_H
#define ASTEROIDS_ENTITY_H

#include "SFML/Graphics.hpp"
#include "constants.h"

class Entity {
protected:
//    sf::Shape* shape;
    std::unique_ptr<sf::Shape> shape;
    float dx = 0.f, dy = 0.f;
    float x = constants::window_width / 2.0f, y = constants::window_height / 2.0f;
    float angle = 0.f;
    float size = 20.0f;
    float speed = 1.0f;
    double ratio = 1.0f;
    bool destroyed{false};
public:
    Entity() : shape(nullptr) {}
//    virtual ~Entity() {
//        delete shape;
//        shape = nullptr;
//    }
    virtual ~Entity() = default;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    sf::FloatRect get_bounding_box() const noexcept;
    sf::Vector2f get_centre() const noexcept;
    bool intersects(const Entity& other) const;

    // Helper functions to get the position of the sprite
    float getX();
    float getY();
    float getDx();
    float getDy();
    float getSpeed();
    float getAngle();
    void setX(float x);
    void setY(float y);
    void setDx(float dx);
    void setDy(float dy);
    void setSpeed(float speed);
    void setAngle(float angle);

    // Helper functions for the state of the entity
    virtual void destroy() noexcept;
    bool is_destroyed() const noexcept;
    virtual std::vector<std::unique_ptr<Entity>> onDestroy();
    virtual std::vector<std::unique_ptr<Entity>> onDestroy(long &score);

};

#endif //ASTEROIDS_ENTITY_H
