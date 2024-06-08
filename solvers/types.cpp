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
