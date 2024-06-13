cmake_minimum_required(VERSION 3.27)
project(Asteroids)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cc entity.cc game.cc
        game.h
        constants.h
        game.cc
        player.h
        player.cc
        bullet.h
        bullet.cc
        asteroid.h
        asteroid.cc
        gamepanel.h
        gamepanel.cc
)
add_executable(Asteroids ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Asteroids sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
