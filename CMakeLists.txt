cmake_minimum_required(VERSION 3.17)
project(console_game)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(console_game main.cpp models/game_obj.h models/player.h models/item.h models/enemy.h models/game_living_obj.h models/game_menu.h)
target_link_libraries(console_game ${CURSES_LIBRARIES})