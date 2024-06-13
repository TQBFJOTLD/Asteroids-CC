#include "entity.h"
#include <iostream>

sf::FloatRect Entity::get_bounding_box() const noexcept {
    return shape ? shape->getGlobalBounds() : sf::FloatRect();
}

sf::Vector2f Entity::get_centre() const noexcept {
    auto box = get_bounding_box();
    return {box.width/2.0f, box.height/2.0f};
}

bool Entity::intersects(const Entity& other) const {
    return shape && other.shape && get_bounding_box().intersects(other.get_bounding_box());
}

float Entity::getX()  {
    return x;
}

float Entity:: getY()  {
    return y;
}

float Entity::getDx() {
    return dx;
}

float Entity::getDy() {
    return dy;
}

float Entity::getSpeed() {
    return speed;
}

float Entity::getAngle() {
    return angle;
}

void Entity::setX(float x) {
    this->x = x;
}

void Entity::setY(float y) {
    this->y = y;
}

void Entity::setDx(float dx) {
    this->dx = dx;
}

void Entity::setDy(float dy) {
    this->dy = dy;
}

void Entity::setSpeed(float speed) {
    this->speed = speed;
}

void Entity::setAngle(float angle) {
    this->angle = angle;
}

// Helper functions for the state of the Entity
void Entity::destroy() noexcept { destroyed = true; }
bool Entity::is_destroyed() const noexcept { return destroyed; }
std::vector<std::unique_ptr<Entity>> Entity::onDestroy() {
    return {};
}
std::vector<std::unique_ptr<Entity>> Entity::onDestroy(long &score) {
    return {};
}
