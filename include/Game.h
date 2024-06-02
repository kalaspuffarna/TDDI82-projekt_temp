#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "State.h"

class Game {
public:
    Game();
    void start();
    void initialize_states();
    void restart_game();
private:
    sf::RenderWindow window;
    std::map<int, std::unique_ptr<State>> states;
    int current_state;
    bool running;
    bool first_start;
    void config_game();
    void handle_events();
    void delay();
};

#endif
