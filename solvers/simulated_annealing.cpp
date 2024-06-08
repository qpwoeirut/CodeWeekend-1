#include <bits/stdc++.h>

#include "types.cpp"

using namespace std;
using pii = pair<int, int>;

// TODO: do some compiler tricks to make this a constexpr on a per-test basis
// and consider getting the inputs at compile time
const int MAX_N = 3000;

// Parameters to tune
const double TEMP_START = 10;
const double TEMP_END = 0.001;
const double INSERT_CHANCE = 0.25;  // Insert a monster in the order.
const double DELETE_CHANCE = 0.25;  // Delete a monster from the order.
const double REVERSE_CHANCE = 0.25;  // Reverse a subarray of the order.
const double SHIFT_CHANCE = 0.25;  // Swap the locations of two subarrays in the order.
const int MISSING_PENALTY = 10;

Hero hero;
Game game;
Monster monster[MAX_N];

int get_moves_between(pii current_position, pii goal_position, int current_speed)
{
    return 0; //TODO: fix this thing because its important
}

const int POINT_THRESHOLD = 50;
const int INF = 1e9 + 7;
const double PI = acos(-1);


pii get_maximal_path(int x, int y, int next_monster, vector<int> monster_order, int num_steps_left, int current_gold)
{
    //pick 50 random points on the circumference of the circle of the next monster, then check for each point what the total number of steps to get to the monster after will be
    pair<pii, pii> best_point = {{INF, INF}, {INF, INF}};
    
    for (int i = 0; i < POINT_THRESHOLD; i++)
    {
        int theta = rnd(0, 359);
        pii final_point = {monster[monster_order[next_monster]].x + hero.get_range() * cos((double)theta * PI/180), monster[monster_order[next_monster]].y + hero.get_range() * sin((double)theta * PI/180)};
        //calculate the number of moves necessary to get to that point and kill the monster
        int necessary_moves = get_moves_between({x,y}, final_point, hero.get_speed()) + (monster[monster_order[next_monster]].hp + hero.get_power() - 1)/hero.get_power();
        if (necessary_moves == best_point.first.first)
        {
            //check whether the distance to the next point is smaller
            if (next_monster + 1 == (int)monster_order.size())
            {
                continue;
            }
            int next_distance = get_moves_between(final_point, {monsters[monster_order[next_monster + 1]].x, monsters[monster_order[next_monster + 1]].y}, hero.get_prosp_speed(monster[next_monster].exp));
            if (next_distance < best_point.first.second)
            {
                best_point = {{necessary_moves, next_distance}, final_point};
            }
            best_point = {necessary_moves, (next_monster + 1 == (int)monster_order.size() ? -1: };
            
        } else if (necessary_moves < best_point.first.first)
        {
            //replace the best point with this point
            best_point = {{necessary_moves, (next_monster + 1 == (int)monster_order.size() ? -1: get_moves_between(final_point, {monsters[monster_order[next_monster + 1]].x, monsters[monster_order[next_monster + 1]].y}, hero.get_prosp_speed(monster[next_monster].exp))}, final_point};
        }
    }
    
    if (num_steps_left < best_point.first.first)
    {
        return {current_gold, (int)monster_order.size() - 1 - next_monster};
    } 
    
    num_steps_left -= best_point.first.first;
    current_gold += monster[next_monster].gold;
    
    if (next_monster + 1 == (int)monster_order.size())
    {
        return {current_gold, 0};
    }
    return get_maximal_path(best_point.second.first, best_point.second.second, next_monster + 1, monster_order, num_steps_left, current_gold);
}

pii calculate_order_score(const vector<int>& order)
{
    return get_maximal_path(start_x, start_y, 0, monster_order, num_turns, 0);
}

vector<Action> recover_moves(const vector<int>& order);

// FIXME: write mutate(order) and rng() functions

vector<Action> simulated_annealing(int iterations) {
    int best_score = 0;
    vector<int> best_order;

    vector<int> order;
    for (int i=0; i<iterations; ++i) {
        // TODO: consider early termination if no new best is found after a while
        const vector<int> new_order = mutate(order);

        const pii new_result = calculate_order_score(new_order);
        const int new_score = new_result.first - new_result.second * MISSING_PENALTY;

        if (best_score < new_score) {
            // TODO: consider printing out the new best order to track progress
            best_score = new_score;
            best_order = new_order;
            order = new_order;  // TODO: try swapping instead?
        } else {
            const double progress = (double)(i) / iterations;
            const double temp = TEMP_START * pow(TEMP_END / TEMP_START, progress);
            if (rng() < exp((new_score - best_score) / temp)) {
                order = new_order;  // TODO: try swapping instead?
            }
        }
    }

    return recover_moves(best_order);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    cin >> hero;
    cin >> game;
    for (int i=0; i<game.num_monsters; ++i) {
        monster[i].id = i;
        cin >> monster[i];
    }

    const vector<Action> answer = simulated_annealing(100000);
    for (const Action& action: answer) {
        cout << action << '\n';
    }
}
