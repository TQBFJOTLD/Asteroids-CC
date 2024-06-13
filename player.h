#ifndef ASTEROIDS_PLAYER_H
#define ASTEROIDS_PLAYER_H

#include "entity.h"
#include "constants.h"
#include "bullet.h"

class Player: public Entity {
public:
    static int lives;
    static const size_t typeHash;
    Player(float x, float y);
    void update() override;
    void draw(sf::RenderWindow& window) override;
    void destroy() noexcept override;
};

#endif //ASTEROIDS_PLAYER_H
