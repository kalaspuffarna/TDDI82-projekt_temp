#include "../include/Game.h"
#include <iostream>

Game::Game() : window{sf::VideoMode{static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)}, "Projekt", sf::Style::Fullscreen},
              states{},
              current_state{START_STATE},
              running{true},
              first_start{true}
{
   initialize_states();
}

void Game::initialize_states() {
   states.clear();
   states.insert(std::pair<int, std::unique_ptr<State>>({START_STATE, std::make_unique<StartState>()}));
   states.insert(std::pair<int, std::unique_ptr<State>>({GAME_STATE, std::make_unique<GameState>()}));
   states.insert(std::pair<int, std::unique_ptr<State>>({END_STATE, std::make_unique<EndState>()}));
}

void Game::start() {
   bool do_once{false};
   bool restart_flag{false};

   config_game();

   while(running) {

       if(current_state == END_STATE) {
           GameState* GamePtr{dynamic_cast<GameState*>(states[GAME_STATE].get())};
           dynamic_cast<EndState*>(states[END_STATE].get())->set_score(GamePtr->get_score());
           states.at(END_STATE)->handle();

           
           if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
               restart_flag = true;
               current_state = START_STATE;
           }
           
       }

       if(!do_once && current_state == GAME_STATE) {
           StartState* startStatePtr{dynamic_cast<StartState*>(states[START_STATE].get())};
           dynamic_cast<GameState*>(states[GAME_STATE].get())->set_powerup(startStatePtr->get_chosen_weapon());
           states.at(GAME_STATE)->handle();
           do_once = true;
       }

       if(restart_flag) {
           restart_game();
           restart_flag = false;
           do_once = false;
       }

       handle_events();
       states.at(current_state)->update();
       window.clear();
       states.at(current_state)->render(window);
       window.display();
       current_state = states.at(current_state)->get_next_state();
   }
}

void Game::restart_game() {
   initialize_states();
   config_game();
   current_state = START_STATE;
}

void Game::config_game() {
   states.at(START_STATE)->handle();
   states.at(END_STATE)->handle();
}

void Game::handle_events() {
   sf::Event event;
   while(window.pollEvent(event)) {
       if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
           running = false;
       }
       states.at(current_state)->handle_event(event);
   }
}
