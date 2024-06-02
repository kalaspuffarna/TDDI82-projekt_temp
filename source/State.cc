#include "../include/State.h"
#include "../include/Entity.h"
#include "../include/rcs.h"
#include <iostream>
#include <typeinfo>
#include <algorithm>

GameState::GameState()
    : Entity_vector{},
      old_ev_size{2},
      wave_size{2},
      background_texture{},
      background{},
      clock{},
      spawn_clock{},
      level_text{},
      level_font{},
      end_game{},
      powerup{0},
      score{}
{
    if (!level_font.loadFromFile("resourses/font/RunningStartBasic.ttf"))
    { 
    }
    background.setTexture(Util::Manager::load("resourses/sprites/background.png"));
    background.setTextureRect(sf::IntRect{0, 0, WIDTH, HEIGHT});
}

StartState::StartState() : play{false},
                           pixels_traveled{},
                           chosen_weapon{},
                           startscreen{},
                           startscreen2{},
                           powerup1{},
                           powerup2{},
                           powerup3{},
                           font{},
                           text{},
                           powerup1_text{},
                           powerup2_text{},
                           powerup3_text{},
                           start_text{},
                           start_clock{},
                           view{}
{
    if (!font.loadFromFile("resourses/font/JMH_Typewriter.ttf"))
    { std::cout << "not loading font" << std::endl;
    }

    startscreen.setTexture(Util::Manager::load("resourses/sprites/startscreen.png"));
    startscreen.setTextureRect(sf::IntRect{0, 0, WIDTH, HEIGHT});

    startscreen2.setTextureRect(sf::IntRect{-WIDTH, 0, WIDTH, HEIGHT});
    startscreen2.setTexture(Util::Manager::load("resourses/sprites/sigma.jpg"));
    startscreen2.setPosition(-WIDTH, 0);

    powerup1.setTexture(Util::Manager::load("resourses/sprites/scythe.png"));
    powerup1.setTextureRect(sf::IntRect{0, 0, 200, 200});
    powerup1.setPosition(WIDTH/6, HEIGHT/2);
    config_start_state(powerup1_text, "Sword", 40, WIDTH/6, HEIGHT/1.3);

    powerup2.setTexture(Util::Manager::load("resourses/sprites/circle.png"));
    powerup2.setTextureRect(sf::IntRect{0, 0, 248, 248});
    powerup2.setPosition(WIDTH/2.2, HEIGHT/2);
    powerup2.setColor(sf::Color::Red);
    config_start_state(powerup2_text, "Circle", 40, WIDTH/2.2, HEIGHT/1.3);

    powerup3.setTexture(Util::Manager::load("resourses/sprites/slash.png"));
    powerup3.setTextureRect(sf::IntRect{0, 0, 136, 68});
    powerup3.setPosition(WIDTH/1.4, HEIGHT/2);
    powerup3.setColor(sf::Color::Green);
    config_start_state(powerup3_text, "Slash", 40, WIDTH/1.4, HEIGHT/1.3);
}

void StartState::config_start_state(sf::Text &text_var, std::string text, int text_size, float WIDTH, float HEIGHT) {
    text_var.setFont(font);
    text_var.setString(text);
    text_var.setCharacterSize(text_size);
    text_var.setPosition(WIDTH, HEIGHT);
}

void GameState::set_powerup(int num) {
    powerup = num;
}

void GameState::handle()
{
    Entity_vector.push_back(std::make_unique<Hero>());
    if (powerup == 0)
    {
        Entity_vector.push_back(std::make_unique<Sythe>());
    }
    else if (powerup == 1)
    {
        Entity_vector.push_back(std::make_unique<Garlic>());
    }
    else if (powerup == 2)
    {
        Entity_vector.push_back(std::make_unique<Whip>());
    }
    for (int i{0}; i < wave_size; ++i)
    {
        Entity_vector.push_back(std::make_unique<Enemy>());
    }
    level_text.setCharacterSize(24);
    level_text.setFillColor(sf::Color::Blue);
    level_text.setFont(level_font);
}

void GameState::enemy_spawner()
{
    if (static_cast<int>(spawn_clock.getElapsedTime().asSeconds()) % 10 == 0 && static_cast<int>(spawn_clock.getElapsedTime().asSeconds()) != 0)
    {
        wave_size += 2;
        old_ev_size = Entity_vector.size();
        for (int i{0}; i < wave_size; ++i)
        {
            Entity_vector.push_back(std::make_unique<Enemy>());
        }
        for (long unsigned int i{old_ev_size}; i < Entity_vector.size(); ++i)
        {
            dynamic_cast<Enemy*>(Entity_vector.at(i).get())->spawn();
        }
        spawn_clock.restart();
    }
}
void GameState::enemy_movement(Hero* const & h)
{
    for (long unsigned int i{2}; i < Entity_vector.size(); ++i)
    {
        dynamic_cast<Enemy*>(Entity_vector.at(i).get())->move(h->get_sprite().getPosition());
    } 
}

void GameState::projectile_spawner(Hero* const & h,Powerups* const & p)
{
    if (p->get_active())
    {
        for (long unsigned int i{2}; i < Entity_vector.size(); ++i)
        {
            dynamic_cast<Enemy*>(Entity_vector.at(i).get())->spawn();
        }
        p->spawn(*h);
        p->active_switch();
    }
}

void GameState::update()
{
    Hero *h{dynamic_cast<Hero *>(Entity_vector.at(0).get())};
    Powerups *p{dynamic_cast<Powerups *>(Entity_vector.at(1).get())};
    sf::Vector2f hero_pos{h->get_sprite().getPosition()};
    enemy_spawner();
    enemy_movement(h);
    projectile_spawner(h, p);

    for (auto& entity1 : Entity_vector) {
        for (auto& entity2 : Entity_vector) {
            if (entity1 != entity2 && entity1->get_sprite().getGlobalBounds().intersects(entity2->get_sprite().getGlobalBounds())) {
                entity1->handleCollision(entity2.get(), clock);
                entity2->handleCollision(entity1.get(), clock);
            }
        }
    }
    kill(h);
    if (h->get_levelup())
    {
        p->upgrade();
        h->levelup_off();
    }
    h->move();
    p->ability(*h);

    for(auto &v : Entity_vector)
    {
        v->update_health_bar();
    }

    level_text.setPosition(h->get_position().x - 70, h->get_position().y + 80);
    level_text.setString("level " + std::to_string(h->get_level()));

    if(h->get_health() <= 0) {end_game = true;};
}
void GameState::kill(Hero* const& h)
{

    if (Entity_vector.size() > 2)
    {
        for (long unsigned int i{2}; i < Entity_vector.size(); ++i)
        {
            if (Entity_vector.at(i).get()->get_currhealth() <= 0)
            {
                auto it{Entity_vector.begin()};
                for (long unsigned int j{0}; j < i; ++j)
                {
                    it++;
                }
                Entity_vector.erase(it);
                h->add_xp(25);
                score++;
            }
        }
    }
}

void GameState::render(sf::RenderTarget &target)
{
    target.draw(background);
    for (auto const& e :Entity_vector)
    {
        target.draw(e->get_sprite());
        target.draw(e->get_healthbox());
        target.draw(e->get_healthbar());
    }   
    target.draw(level_text);
}

sf::RenderWindow *GameState::get_window()
{
    sf::RenderWindow *test{};
    return test;
}

int GameState::get_next_state()
{
    if (end_game)
    {
        end_game = false;
        return END_STATE;
    }
    return GAME_STATE;
}

void GameState::handle_event(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Key::Escape) {
            end_game = true;
        }
    }   
}

int GameState::get_score() {
    return score;
}

void StartState::handle()
{
    view.reset(sf::FloatRect{0, 0, static_cast<float>(WIDTH), static_cast<float>(HEIGHT)});
    text.setString("GAME START");
    text.setFont(font);
    text.setCharacterSize(96);
    text.setFillColor(sf::Color(173, 0, 255));
    auto bounds{text.getGlobalBounds()};
    text.setPosition((WIDTH - bounds.width) / 2, (HEIGHT - bounds.height) / 5);

    start_text.setString("Select your weapon!");
    start_text.setFont(font);
    start_text.setCharacterSize(48);
    start_text.setFillColor(sf::Color(173, 0, 255));
    bounds = start_text.getGlobalBounds();
    start_text.setPosition((WIDTH - bounds.width) / 2, (HEIGHT - bounds.height) / 3);
}

void StartState::render(sf::RenderTarget &target)
{
    target.setView(view);
    target.draw(startscreen);
    target.draw(startscreen2);
    target.draw(powerup1);
    target.draw(powerup2);
    target.draw(powerup3);
    target.draw(powerup1_text);
    target.draw(powerup2_text);
    target.draw(powerup3_text);
    target.draw(text);
    target.draw(start_text);
}

void StartState::update()
{
    sf::FloatRect MouseBox{static_cast<float>(sf::Mouse::getPosition().x), static_cast<float>(sf::Mouse::getPosition().y), 10, 10};

    if(MouseBox.intersects(powerup1_text.getGlobalBounds())) {
        powerup1_text.setFillColor(sf::Color::Red);
    } else if(MouseBox.intersects(powerup2_text.getGlobalBounds())) {
        powerup2_text.setFillColor(sf::Color::Red);
    } else if(MouseBox.intersects(powerup3_text.getGlobalBounds())) {
        powerup3_text.setFillColor(sf::Color::Red);
    } else {
        powerup1_text.setFillColor(sf::Color::White);
        powerup2_text.setFillColor(sf::Color::White);
        powerup3_text.setFillColor(sf::Color::White);
    }
}

void StartState::handle_event(sf::Event event)
{
    sf::FloatRect MouseBox{static_cast<float>(sf::Mouse::getPosition().x), static_cast<float>(sf::Mouse::getPosition().y), 10, 10};

    bool Intersection1 = MouseBox.intersects(powerup1_text.getGlobalBounds());
    bool Intersection2 = MouseBox.intersects(powerup2_text.getGlobalBounds());
    bool Intersection3 = MouseBox.intersects(powerup3_text.getGlobalBounds());
    if(event.type == sf::Event::MouseButtonPressed) {
        if(Intersection1) {
            play = true;
            chosen_weapon = 0;
        } else if(Intersection2) {
            play = true;
            chosen_weapon = 1;
        } else if(Intersection3) {
            play = true;
            chosen_weapon = 2;
        }
    }
}

int StartState::get_chosen_weapon() const {
    return chosen_weapon;
}

int StartState::get_next_state()
{
    if (play)
    {
        play = false;
        return GAME_STATE;
    }
    else
    {
        return START_STATE;
    }
}

// ENDSTATE

EndState::EndState() : start_over{false}, endscreen{}, font{}, text{}, text2{}, score{} {
    if (!font.loadFromFile("resourses/font/JMH_Typewriter.ttf")) {std::cout << "not loading font" << std::endl;}
    endscreen.setTexture(Util::Manager::load("resourses/sprites/endscreen.png"));
    endscreen.setTextureRect(sf::IntRect{0, 0, WIDTH, HEIGHT});
}




void EndState::handle() {
    text.setString("L + Ratio: " + std::to_string(score));
    text.setFont(font);
    text.setCharacterSize(96);
    text.setFillColor(sf::Color(173, 0, 255));
    auto bounds{text.getGlobalBounds()};
    text.setPosition((WIDTH - bounds.width) / 2, (HEIGHT - bounds.height) / 2);

    text2.setString("Please click <Escape> to segfault!\nor press <Enter> to play again");
    text2.setFont(font);
    text2.setCharacterSize(48);
    text2.setFillColor(sf::Color(173, 0, 255));
    bounds = text2.getGlobalBounds();
    text2.setPosition((WIDTH - bounds.width) / 2, (HEIGHT - bounds.height) / 1.5);

    
}

void EndState::update() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        start_over = true;
    }
}

void EndState::render(sf::RenderTarget & target) {
    target.draw(endscreen);
    target.draw(text);
    target.draw(text2);
}

int EndState::get_next_state() {
    if(start_over) {
        start_over = false;
        return START_STATE;
    } else {
        return END_STATE;
    }
}

void EndState::handle_event(sf::Event event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Key::Escape) {
            start_over = true;
        }
    }
}

void EndState::set_score(int num) {
    score = num;
}
