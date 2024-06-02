#include "../include/Entity.h"
#include "../include/rcs.h"
#include "../include/constants.h"
#include <cmath>
#include <iostream>


Entity::Entity() :  speed{0},
                    curr_health{0},
                    max_health{0},
                    damage{15},
                    pos{},
                    radius{50},
                    sprite{},
                    texture{},
                    health_box{},
                    health_bar{}
{}

Entity::Entity(int init_health)  :  speed{0},
                                    curr_health{init_health},
                                    max_health{init_health},
                                    damage{15},
                                    pos{},
                                    radius{50},
                                    sprite{},
                                    texture{},
                                    last_hit_timestamp{},
                                    health_box{sf::Vector2f(50, 5)},
                                    health_bar{health_box.getSize()}
{
}
Entity::~Entity()
{
}

int Entity::get_currhealth() {
    return curr_health;
}

void Entity::update_health_bar()
{
    if(max_health > 0 )
    {
        health_bar.setSize(sf::Vector2f(50 * (static_cast<float>(curr_health) / static_cast<float>(max_health)), 5));
        health_box.setPosition(sprite.getPosition().x - 25, sprite.getPosition().y - 100);
        health_bar.setPosition(health_box.getPosition());
    }
}

sf::RectangleShape Entity::get_healthbox() const
{
    return health_box;
}

sf::RectangleShape Entity::get_healthbar() const
{
    return health_bar;
}

void Hero::handleCollision(Entity *other, sf::Clock& c) {
    if (dynamic_cast<Enemy*>(other) && c.getElapsedTime().asSeconds() - last_hit_timestamp.asSeconds() > 1) {
        take_damage();
        set_last_hit_timestamp(c.getElapsedTime());
    }
}

void Enemy::handleCollision(Entity *other, sf::Clock& c) {
    if (dynamic_cast<Enemy*>(other)) {
        knockback(*dynamic_cast<Enemy*>(other)); 
    } else if (dynamic_cast<Powerups*>(other)) {
        if (c.getElapsedTime().asSeconds() - last_hit_timestamp.asSeconds() > 1) {
            do_damage();
            if (curr_health > 0 && !dynamic_cast<Garlic*>(other)) {
                knockback();
            }
            set_last_hit_timestamp(c.getElapsedTime());
        }
    }
}

void Powerups::handleCollision(Entity *other, sf::Clock& c) {
    other->get_sprite().getGlobalBounds();
    c.getElapsedTime();
}

Hero::Hero()
    : Entity(100), current_movement{}, curr_XP{0}, max_XP{100}, level{1}, level_up{false}  
{
    sprite.setTexture(Util::Manager::load("resourses/sprites/hero.png"));
    sprite.setPosition(1000, 400);
    sprite.setOrigin(240, 240);
    sprite.setScale(0.3, 0.3);
    update_health_bar();
    health_bar.setFillColor(sf::Color::Green);
    health_box.setFillColor(sf::Color::Red);
}

int Hero::get_health() const {
    return curr_health;
}

sf::Vector2f Entity::get_position() const
{
    return sprite.getPosition();
}

void Entity::sub_health(int hp)
{
    curr_health -= hp;
}

float Entity::get_radius() const
{
    return sprite.getGlobalBounds().width / 2;
}

int Entity::get_damage() const
{
    return 1;
}

void Hero::move()
{
    std::vector<std::pair<sf::Keyboard::Key, sf::Vector2i>> v{
        {sf::Keyboard::Right, {1, 0}},
        {sf::Keyboard::Left, {-1, 0}},
        {sf::Keyboard::Up, {0, -1}},
        {sf::Keyboard::Down, {0, 1}}};
    current_movement.clear();
    for (auto dir : v)
    {
        if (sf::Keyboard::isKeyPressed(dir.first))
        {
            sprite.move(dir.second.x, dir.second.y);
            if(dir.second.x < 0) {
                sprite.setScale(-0.3, 0.3);
            } else {
                sprite.setScale(0.3, 0.3);
            }
            current_movement.push_back(dir.second);
        }
    }
}

sf::Vector2f Hero::get_origin() const {
    return sprite.getOrigin();
}

std::vector<sf::Vector2i> Hero::get_current_movement() const
{
    return current_movement;
}

void Hero::levelup_off()
{
    level_up = false;
}

bool Hero::get_levelup() const 
{
    return level_up;
}

void Hero::add_xp(int const& xp)
{
    curr_XP += xp;
    if (curr_XP >= max_XP)
    {
        curr_XP = 0;
        level_up = true;
        ++level;
        max_XP += 100;
    }
}

int Hero::get_level() const 
{
    return level;
}

int Hero::get_damage() const
{
   return damage*level; 
}

void Hero::take_damage() {
    curr_health -= 10;
}

sf::Sprite Entity::get_sprite() const {
    return sprite;
}

Powerups::Powerups() : active{true}
{
}

bool Powerups::get_active() const
{
    return active;
}

void Powerups::active_switch()
{
    if(active) active =false;
    else active = true;
}

Sythe::Sythe()
{
    sprite.setTexture(Util::Manager::load("resourses/sprites/scythe.png"));
    sprite.setScale(0.5, 0.5);
}
        

void Sythe::spawn(Hero const &hero)
{
    sprite.setOrigin(hero.get_origin().x, hero.get_origin().y + 150);
    sprite.setPosition(hero.get_position().x + 350, hero.get_position().y);
}

void Sythe::ability(Hero const& h)
{
    int speed{5};
    sf::Vector2f movedir;
    for (auto i : h.get_current_movement())
    {
        sprite.move(i.x, i.y); 
    }
    movedir.x = -sin((M_PI / 180) * sprite.getRotation()) * speed;
    movedir.y = cos((M_PI / 180) * sprite.getRotation()) * speed;
    sprite.move(movedir.x, movedir.y);  
    sprite.rotate(1);                   
}

void Sythe::upgrade()
{
    sprite.scale(sf::Vector2f(1.2, 1.2));
}

Garlic::Garlic()
{
    sprite.setTexture(Util::Manager::load("resourses/sprites/circle.png"));
    sprite.scale(sf::Vector2f(1.2, 1.2));
}

void Garlic::spawn(Hero const& h)
{
    sprite.setPosition(h.get_origin());
    sprite.setOrigin(sprite.getGlobalBounds().width/2.0f - 20, sprite.getGlobalBounds().height/2.0f - 20);
}

void Garlic::upgrade()
{
    sprite.scale(sf::Vector2f(1.1, 1.1));
}

void Garlic::ability(Hero const& h)
{
    sprite.setPosition(h.get_position().x, h.get_position().y);
}

Whip::Whip() : side{true}, whip_clock{}, whip_up{true}, scale{}
{
    sprite.setTexture(Util::Manager::load("resourses/sprites/slash.png"));
    sprite.setScale(1.2, 1);
    scale = sf::Vector2f(sprite.getScale());
}

void Whip::spawn(Hero const& h)
{
    sprite.setOrigin(sprite.getGlobalBounds().width/2.0f, sprite.getGlobalBounds().height/2.0f);
    sprite.setPosition(h.get_position().x + sprite.getGlobalBounds().width, h.get_position().y);
}

void Whip::ability(Hero const& h)
{
    if (h.get_sprite().getScale() == sf::Vector2f(0.3, 0.3))
    {
        side = true;
    }
    else if (h.get_sprite().getScale() == sf::Vector2f(-0.3, 0.3))
    {
        side = false;
    }

    if (whip_clock.getElapsedTime().asSeconds() >= 0.5)
    {
        whip_up = !whip_up;
        whip_clock.restart();
    }

    if (whip_up)
    {
        if(side == true)
        {
            sprite.setPosition(h.get_position().x + sprite.getGlobalBounds().width, h.get_position().y);
            sprite.setScale(scale);
        }
        else if(side == false)
        {
            sprite.setPosition(h.get_position().x - sprite.getGlobalBounds().width, h.get_position().y);
            sprite.setScale(-scale.x, scale.x);
        }
    }
    else
    {
        sprite.setPosition(-1000000000000, 1000000000000);
    }
}

void Whip::upgrade()
{
    scale = sf::Vector2f(scale.x * 1.2, scale.y);
}

Enemy::Enemy() : Entity(25), damage_factor{}, current_movement{}
{
    sprite.setTexture(Util::Manager::load("resourses/sprites/enemy.png"));
    //sprite.setTextureRect(sf::IntRect(0, 0, 200, 200));
    sprite.setScale(0.3, 0.3);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    update_health_bar();
    health_bar.setFillColor(sf::Color::Green);
    health_box.setFillColor(sf::Color::Red);
}

void Enemy::spawn()
{
    int seed{WIDTH};
    int seed2{HEIGHT};
    int randnum1{rand() % seed};
    int randnum2{rand() % seed2};
    
    int randnum3{rand() % 3};
   
   if (randnum3 == 0){

       sprite.setPosition(0, randnum2);
   }
   else if(randnum3 == 1){

       sprite.setPosition(randnum1, 0);
   }
   else if(randnum3 == 2){

       sprite.setPosition(randnum2, HEIGHT);
   }
   else if(randnum3 == 3){

       sprite.setPosition(WIDTH, randnum1);
   }

    
}

void Enemy::move(sf::Vector2f const& hero_pos)
{
    sf::Vector2f totalMovement{};
    totalMovement.x = hero_pos.x - sprite.getPosition().x;
    totalMovement.y = hero_pos.y - sprite.getPosition().y;
    float distance = std::sqrt(totalMovement.x * totalMovement.x + totalMovement.y * totalMovement.y);
    float speed = 0.6; // Adjust the speed as desired
    if(totalMovement.x < 0) {
        sprite.setScale(-0.3, 0.3);
    } else {
        sprite.setScale(0.3, 0.3);
    }   
    current_movement = sf::Vector2f(totalMovement.x * speed / distance, totalMovement.y * speed / distance);
    sprite.move(current_movement);
}

void Enemy::knockback()
{
    sprite.move(current_movement.x * -100,current_movement.y * -100);
}

void Enemy::knockback(Enemy const &rhs)
{
    sf::Vector2f totalMovement{};
    totalMovement.x = rhs.get_position().x - get_position().x;
    totalMovement.y = rhs.get_position().y - get_position().y;
    sprite.move(totalMovement.x * -0.1, totalMovement.y * -0.1);
}

void Enemy::do_damage() {
    curr_health -= 10;
}

void Entity::set_last_hit_timestamp(sf::Time const& time)
{
    last_hit_timestamp = time;
}
sf::Time Entity::get_last_hit_timestamp() const
{
    return last_hit_timestamp;
}
