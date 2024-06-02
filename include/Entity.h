#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <vector>

class Entity
{
protected:
    int speed;
    int curr_health;
    int max_health;
    int damage;
    sf::Vector2i pos;
    float radius  = 50;
    sf::Sprite sprite;    // Ändrade från CircleShape så att varje entity kan ha en bild
    sf::Texture texture;
    sf::Time last_hit_timestamp = sf::Time::Zero;
    sf::RectangleShape health_box;
    sf::RectangleShape health_bar;
public:
    Entity();
    Entity(int init_health);
    virtual ~Entity();
    sf::Sprite get_sprite() const;
    int get_currhealth();
    virtual int get_damage() const;
    sf::Vector2f get_position() const;
    float get_radius() const;
    void sub_health(int hp);
    void set_last_hit_timestamp(sf::Time const&);
    sf::Time get_last_hit_timestamp() const;
    void update_health_bar();
    sf::RectangleShape get_healthbox() const;
    sf::RectangleShape get_healthbar() const;
    virtual void handleCollision(Entity *other, sf::Clock& c) = 0;
    
};

//--------------------------------------------Enemy

class Enemy : public Entity
{
public:
    Enemy();
    void get_player_position();
    void spawn();
    void move(sf::Vector2f const& hero_pos);
    void knockback();
    void knockback(Enemy const& rhs);
    void do_damage();
    void handleCollision(Entity *other, sf::Clock& c) override;
protected:
    int damage_factor;
    sf::Vector2f current_movement;
};

// class Boss : public Enemy
// {
// public:
// private:
// };

// class Grunt : public Enemy
// {
// public:
// private:
// };

//--------------------------------------------Hero
class Hero : public Entity
{
public:
    Hero();
    ~Hero() = default;
    void move();
    bool get_levelup() const;
    void levelup_off();
    void add_xp(int const& xp);
    int get_level() const;
    int get_damage() const override;
    int get_health() const;
    void take_damage();
    sf::Vector2f get_origin() const;
    std::vector<sf::Vector2i> get_current_movement() const;
    void handleCollision(Entity *other, sf::Clock& c) override;

private:
    std::vector<sf::Vector2i> current_movement;
    int curr_XP;
    int max_XP;
    int level;
    bool level_up;
};

//--------------------------------------------Powerups
class Powerups : public Entity
{
public:
    Powerups();
    bool get_active() const;
    void active_switch();
    virtual void spawn(Hero const & hero) = 0;
    virtual void upgrade() = 0;
    virtual void ability(Hero const& h) = 0;
    void handleCollision(Entity *other, sf::Clock& c) override;
protected:
    bool active;
};

class Sythe : public Powerups
{
public:
    Sythe();
    void spawn(Hero const & hero) override;
    void ability(Hero const& h) override;
    void upgrade() override;
};

class Garlic : public Powerups
{
public:
    Garlic();
    void spawn(Hero const& h) override;
    void ability(Hero const& h) override;
    void upgrade() override;
};

class Whip : public Powerups
{
public:
    Whip();
    void spawn(Hero const& h) override;
    void ability(Hero const& h) override;
    void upgrade() override;
private:
    bool side;
    sf::Clock whip_clock;
    bool whip_up;
    sf::Vector2f scale;
};

#endif
