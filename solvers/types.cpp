#ifndef TYPES_CPP
#define TYPES_CPP

#include <iostream>

struct Game {
    int start_x, start_y;
    int width, height;
    int num_turns, num_monsters;

    friend std::istream& operator>>(std::istream& is, Game& game) {
        is >> game.start_x >> game.start_y >> game.width >> game.height >> game.num_turns >> game.num_monsters;
        return is;
    }
};

struct Hero {
    int base_speed, base_power, base_range;
    int level_speed_coeff, level_power_coeff, level_range_coeff;
    int level = 0;
    int exp = 0;

    void add_exp(int delta)
    {
        exp += delta;
        if (exp >= 1000 + level * (level - 1) * 50)
        {
            exp -= 1000 + level * (level - 1) * 50;
            level += 1;
        }
    }
    
    void get_prosp_level(int delta)
    {
        int current_level = level;
        int prospective_exp = exp + delta;
        while (prospective_exp >= 1000 + current_level * (current_level - 1) * 50)
        {
            prospective_exp -= 1000 + current_level * (current_level - 1) * 50;
            
        }
    }

    void get_speed(int current_level = level)
    {
        return base_speed + (int)(base_speed * current_level * level_speed_coeff/100);
    }
    void get_range()
    {
        return base_range + (int)(base_range * current_level * level_range_coeff/100);
    }
    void get_power()
    {
        return base_power + (int)(base_power * current_level * level_power_coeff/100);
    }
    
    void get_prosp_speed(int delta)
    {
        return get_speed(get_prosp_level(delta));
    }
    
    void get_prosp_power(int delta)
    {
        return get_power(get_prosp_level(delta));
    }
    
    void get_prosp_range(int delta)
    {
        return get_range(get_prosp_level(delta));
    }

    friend std::istream& operator>>(std::istream& is, Hero& hero) {
        is >> hero.base_speed >> hero.base_power >> hero.base_range >> hero.level_speed_coeff >> hero.level_power_coeff >> hero.level_range_coeff;
        return is;
    }
};

struct Monster {
    int id;
    int x, y;
    int hp, gold, exp;

    friend std::istream& operator>>(std::istream& is, Monster& monster) {
        is >> monster.x >> monster.y >> monster.hp >> monster.gold >> monster.exp;
        return is;
    }
};

struct Action {
    bool is_move;
    int a, b;
    
    friend std::ostream& operator<<(std::ostream& os, const Action& action) {
        if (action.is_move) {
            os << "move " << action.a << ' ' << action.b;
        } else {
            os << "attack " << action.a;
        }
        return os;
    }
};

#endif // TYPES_CPP
