#include "player.h"
#include <iostream>
#include <cmath>

int Player::lives = constants::player_lives;
const size_t Player::typeHash = typeid(Player).hash_code();

Player::Player(float x, float y) : Entity(){
    this->x = x;
    this->y = y;

//    sf::ConvexShape* playerShape = new sf::ConvexShape();
    auto playerShape = std::make_unique<sf::ConvexShape>();
    playerShape->setPointCount(5);


    // define the points
    playerShape->setPoint(0, sf::Vector2f(0.f, 0.f));
    playerShape->setPoint(1, sf::Vector2f(12.f, 40.f));
    playerShape->setPoint(2, sf::Vector2f(7.f, 37.f));
    playerShape->setPoint(3, sf::Vector2f(-7.f, 37.f));
    playerShape->setPoint(4, sf::Vector2f(-12.f, 40.f));
    playerShape->setOutlineColor(sf::Color::White);
    playerShape->setFillColor(sf::Color::Black);
    playerShape->setOutlineThickness(2.0f);
    playerShape->setOrigin({0, 20});
    playerShape->setPosition(x, y);

    shape = std::move(playerShape);
}

void Player::update() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        angle -= constants::rotate_degree;
        angle = fmod(angle + 360.0f, 360.0f);
        shape->setRotation(angle);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        angle += constants::rotate_degree;
        angle = fmod(angle + 360.0f, 360.0f);
        shape->setRotation(angle);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        dx += speed * std::sin(angle * M_PI / 180.0) * 0.3;
        dy -= speed * std::cos(angle * M_PI / 180.0) * 0.3;

        // Limit the player's speed
        float currentSpeed = std::sqrt(dx * dx + dy * dy);
        if (currentSpeed > constants::maximum_speed) {
            dx = (dx / currentSpeed) * constants::maximum_speed;
            dy = (dy / currentSpeed) * constants::maximum_speed;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        // Gradually decrease the speed
        float deceleration = 0.98f;
        dx *= deceleration;
        dy *= deceleration;

        float threshold = 0.01f;
        if (std::abs(dx) < threshold) dx = 0;
        if (std::abs(dy) < threshold) dy = 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

    }

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

    shape->setPosition(x, y);
    shape->setRotation(angle);
}

void Player::destroy() noexcept{
    if (Player::lives > 0) {
        -- lives;
        x = constants::window_width / 2.0f;
        y = constants::window_height / 2.0f;
        dx = dy = angle = 0;
    }
    else destroyed = true;
}

void Player::draw(sf::RenderWindow& window) {
    if (shape)
        window.draw(*shape);
}
