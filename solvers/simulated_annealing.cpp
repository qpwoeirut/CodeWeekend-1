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

pii calculate_order_score(const vector<int>& order);
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
