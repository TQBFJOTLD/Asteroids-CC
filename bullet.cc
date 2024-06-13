#include "bullet.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>

Bullet::Bullet(float x, float y, float radius, float angle, float speed){
//    std::cout << "Bullet created with angle: " << angle << " dx: " << dx << " dy: " << dy << std::endl;
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->speed = speed;
    this->dx = speed * std::sin(angle * M_PI / 180.0);
    this->dy = -speed * std::cos(angle * M_PI / 180.0);

//    shape = new sf::CircleShape(radius);
    shape = std::make_unique<sf::CircleShape>(radius);
    shape->setPosition(x, y);
    shape->setRotation(angle);
    shape->setFillColor(sf::Color::White);

    lifetimeTimer.restart();
}

void Bullet::update() {
    x += dx;
    y += dy;
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
    if (shape) {
        shape->setPosition(x, y);
    }
    if (lifetimeTimer.getElapsedTime().asSeconds() > constants::bullets_lifetime) {
        destroy();
    }
}

void Bullet::draw(sf::RenderWindow& window) {
    if (shape)
        window.draw(*shape);
}