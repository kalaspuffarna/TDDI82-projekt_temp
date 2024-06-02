#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SFML/Graphics.hpp>

const int WIDTH{static_cast<int>(sf::VideoMode::getDesktopMode().width)};
const int HEIGHT{static_cast<int>(sf::VideoMode::getDesktopMode().height)};

const int START_STATE{0};
const int GAME_STATE{1};
const int END_STATE{2};

#endif
