#ifndef ASTEROIDS_CONSTANTS_H
#define ASTEROIDS_CONSTANTS_H

struct constants {
    static constexpr int window_width = 1600;
    static constexpr int window_height = 800;
    static constexpr int player_lives = 2;
    static constexpr int new_asteroids = 2;
    static constexpr int rotate_degree = 5;
    static constexpr int maximum_speed = 5;
    static constexpr float LARGE_RATIO = 5, MEDIUM_RATIO = 3, SMALL_RATIO = 1;
    static constexpr int asteroids_num = 1;
    static constexpr float bullets_lifetime = 3;
    static constexpr float bullets_cooldown = 0.1;
    static constexpr int LARGE_SCORE = 900, MEDIUM_SCORE = 600, SMALL_SCORE = 300;
    static constexpr int HIGH_SCORE_NUMS = 10;
};


#endif //ASTEROIDS_CONSTANTS_H
