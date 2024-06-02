#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "constants.h"
#include "Entity.h"
#include <map>

class State {
public:
    virtual ~State() = default;
    virtual void handle() = 0;
    virtual void update() = 0;
    virtual void render(sf::RenderTarget & target) = 0;
    virtual int get_next_state() = 0;
    virtual void handle_event(sf::Event event) = 0;
};

class GameState : public State {
public:
    GameState(); // Konstruktor
    ~GameState() = default;
    virtual void handle() override;
    void enemy_spawner();
    void enemy_movement(Hero* const& h);
    void projectile_spawner(Hero* const & h,Powerups* const & p);
    virtual void update() override;
    virtual void render(sf::RenderTarget & target) override;
    virtual int get_next_state() override;
    sf::RenderWindow* get_window();
    virtual void handle_event (sf::Event event) override;
    void kill(Hero* const &);
    void set_powerup(int num);
    int get_score();
private:
    std::vector<std::unique_ptr<Entity>> Entity_vector;
    long unsigned int old_ev_size;
    int wave_size;
    sf::Texture background_texture;
    sf::Sprite background;
    sf::Clock clock;
    sf::Clock spawn_clock;
    sf::Text level_text;
    sf::Font level_font;
    bool end_game;
    int powerup;
    int score;
};

class StartState : public State {
public:
    StartState();
    ~StartState() = default;
    virtual void handle() override;
    virtual void update() override;
    virtual void render(sf::RenderTarget & target) override;
    virtual int get_next_state() override;
    virtual void handle_event (sf::Event event) override;
    void config_start_state(sf::Text &text_var, std::string text, int text_size, float WIDTH, float HEIGHT);
    int get_chosen_weapon() const;
private:
    bool play;
    int pixels_traveled;
    int chosen_weapon;
    sf::Sprite startscreen;
    sf::Sprite startscreen2;
    sf::Sprite powerup1;
    sf::Sprite powerup2;
    sf::Sprite powerup3;
    sf::Font font;
    sf::Text text;
    sf::Text powerup1_text;
    sf::Text powerup2_text;
    sf::Text powerup3_text;
    sf::Text start_text;
    sf::Clock start_clock;
    sf::View view;
};

class EndState : public State {
public:
    EndState();
    ~EndState() = default;
    virtual void handle() override;
    virtual void update() override;
    virtual void render(sf::RenderTarget & target) override;
    virtual int get_next_state() override;
    virtual void handle_event (sf::Event event) override;
    void set_score(int num);
private:
    bool start_over;
    sf::Sprite endscreen;
    sf::Font font;
    sf::Text text;
    sf::Text text2;
    int score;
};

#endif
