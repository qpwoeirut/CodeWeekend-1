#include <bits/stdc++.h>

#include "rng.cpp"
#include "types.cpp"

using namespace std;
using pii = pair<int, int>;

// Parameters to tune
constexpr const double TEMP_START = 10;
constexpr const double TEMP_END = 0.001;
constexpr const double REVERSE_CHANCE = 0.5;  // Reverse a subarray of the order.
constexpr const double SHIFT_CHANCE = 0.5;  // Swap the locations of two subarrays in the order.
constexpr const int MISSING_PENALTY = 10;

constexpr const int MAX_LEVEL = 25;  // Only tests #36 and #37 are theoretically higher and those are probably not reachable.

static_assert(REVERSE_CHANCE + SHIFT_CHANCE == 1.0);


// TODO: do some compiler tricks to make this a constexpr on a per-test basis
// and consider getting the inputs at compile time
const int MAX_N = 2858;
const int MAX_W = 3000;
const int MAX_H = 2222;

Hero hero;
Game game;
Monster monster[MAX_N];

inline int square(const int x) {
    return x * x;
}

// TODO: try to consteval this
int dist[MAX_LEVEL + 1][MAX_H][MAX_W];
vector<pii> reachable[MAX_LEVEL + 1];
void calculate_dist() {
    hero.reset();
    for (int level = 0; level <= MAX_LEVEL; ++level) {
        for (int x=0; x<MAX_W; ++x) {
            for (int y=0; y<MAX_H; ++y) {
                dist[level][y][x] = MAX_W * MAX_H;
            }
        }

        for (int x=-MAX_W + 1; x < MAX_W; ++x) {
            for (int y=-MAX_H + 1; y < MAX_H; ++y) {
                if (x * x + y * y <= square(hero.get_range(level))) reachable[level].emplace_back(y, x);
            }
        }
        reachable[level].shrink_to_fit();

        cerr << "level " << level << ' ' << reachable[level].size() << endl;

        // Sort by descending distance because why not.
        sort(reachable[level].begin(), reachable[level].end(), [](const pii& a, const pii& b) {
            return square(a.first) + square(a.second) > square(b.first) + square(b.second);
        });

        queue<pii> q;
        q.emplace(0, 0);
        dist[level][0][0] = 0;
        while (q.size() > 0) {
            const pii cur = q.front(); q.pop();
            
            if ((cur.first + 1 < MAX_H && dist[level][cur.first][cur.second] != dist[level][cur.first + 1][cur.second])
                 || (cur.second + 1 < MAX_W && dist[level][cur.first][cur.second] != dist[level][cur.first][cur.second + 1])) {
                for (const pii& d: reachable[level]) {
                    if (d.first >= 0 && d.second >= 0 && cur.first + d.first < MAX_H && cur.second + d.second < MAX_W && dist[level][cur.first + d.first][cur.second + d.second] > dist[level][cur.first][cur.second] + 1) {
                        dist[level][cur.first + d.first][cur.second + d.second] = dist[level][cur.first][cur.second] + 1;
                        q.emplace(cur.first + d.first, cur.second + d.second);
                    }
                }
            }
        }
    }
}

pii calculate_order_score(const vector<int>& order) {
    hero.reset();

    int x = game.start_x, y = game.start_y;
    int turns = 0;
    int gold = 0;
    int i = 0;
    while (i < order.size()) {
        const int dx = monster[order[i]].x - x, dy = monster[order[i]].y - y;
        if (dx * dx + dy * dy <= hero.get_range() * hero.get_range()) {
            turns += (monster[order[i]].hp + hero.get_power() - 1) / hero.get_power();
            if (turns >= game.num_turns) return pii(gold, order.size() - i);

            gold += monster[order[i]].gold;
            hero.add_exp(monster[order[i]].exp);
            ++i;
            continue;
        }
        const pii best = *min_element(reachable[hero.level].begin(), reachable[hero.level].end(), [&i, &order, &x, &y](const pii& a, const pii& b) {
            // TODO: this doesn't account for leveling up before attacking the next monster(s).
            for (int j=i; j<order.size(); ++j) {
                const int dx_a = abs(monster[order[j]].x - (x + a.second));
                const int dy_a = abs(monster[order[j]].y - (y + a.first));
                const int dx_b = abs(monster[order[j]].x - (x + b.second));
                const int dy_b = abs(monster[order[j]].y - (y + b.first));
                if (dist[hero.level][dy_a][dx_a] != dist[hero.level][dy_b][dx_b]) {
                    return dist[hero.level][dy_a][dx_a] < dist[hero.level][dy_b][dx_b];
                }
            }
            return false;
        });

        x += best.second;
        y += best.first;
        ++turns;

        if (turns >= game.num_turns) return pii(gold, order.size() - i);
    }
    return pii(gold, 0);
}

vector<Action> recover_actions(const vector<int>& order) {
    hero.reset();

    int x = game.start_x, y = game.start_y;
    vector<Action> actions;
    int gold = 0;
    int i = 0;
    while (i < order.size()) {
        const int dx = monster[order[i]].x - x, dy = monster[order[i]].y - y;
        if (dx * dx + dy * dy <= hero.get_range() * hero.get_range()) {
            for (int p=0; p<monster[order[i]].hp; p += hero.get_power()) actions.emplace_back("attack", order[i]);
            if (actions.size() >= game.num_turns) {
                while (actions.size() > game.num_turns) actions.pop_back();
                return actions;
            }

            gold += monster[order[i]].gold;
            hero.add_exp(monster[order[i]].exp);
            ++i;
            continue;
        }
        const pii best = *min_element(reachable[hero.level].begin(), reachable[hero.level].end(), [&i, &order, &x, &y](const pii& a, const pii& b) {
            // TODO: this doesn't account for leveling up before attacking the next monster(s).
            for (int j=i; j<order.size(); ++j) {
                const int dx_a = abs(monster[order[j]].x - (x + a.second));
                const int dy_a = abs(monster[order[j]].y - (y + a.first));
                const int dx_b = abs(monster[order[j]].x - (x + b.second));
                const int dy_b = abs(monster[order[j]].y - (y + b.first));
                if (dist[hero.level][dy_a][dx_a] != dist[hero.level][dy_b][dx_b]) {
                    return dist[hero.level][dy_a][dx_a] < dist[hero.level][dy_b][dx_b];
                }
            }
            return false;
        });

        x += best.second;
        y += best.first;
        actions.emplace_back("move", x, y);

        if (actions.size() >= game.num_turns) return actions;
    }
    return actions;
}

Rng rng(8);
vector<int> mutate(vector<int> order, const int n_used) {  // pass in a copy to mutate
    int idx1 = rng.next_int(n_used);
    int idx2 = rng.next_int(order.size());
    while (idx1 == idx2) idx2 = rng.next_int(order.size());
    if (idx1 > idx2) swap(idx1, idx2);

    const double choice = rng.next_double();
    if (choice < REVERSE_CHANCE) {
        for (int i=0; i+i<idx2 - idx1; ++i) {
            swap(order[idx1 + i], order[idx2 - i]);
        }
    } else {
        const int len = rng.next_int(min(idx2 - idx1, (int)(order.size()) - idx2)) + 1;
        for (int i=0; i<len; ++i) {
            swap(order[idx1 + i], order[idx2 + i]);
        }
    }
    return order;
}

vector<Action> simulated_annealing(int attempts, int iterations) {
    int best_score = 0;
    vector<int> best_order;

    for (int attempt=0; attempt<attempts; ++attempt) {
        vector<int> order(game.num_monsters);
        for (int i=0; i<order.size(); ++i) order[i] = i;

        int n_used = 1;
        for (int i=0; i<iterations; ++i) {
            // TODO: consider early termination if no new best is found after a while, signal handling for killing
            const vector<int> new_order = mutate(order, n_used);

            const pii new_result = calculate_order_score(new_order);
            const int new_score = new_result.first;

            if (best_score < new_score) {
                // TODO: consider printing out the new best order to track progress
                best_score = new_score;
                best_order = new_order;
                order = new_order;  // TODO: try swapping instead?
                n_used = new_result.second;

                cerr << "New best score: " << best_score << endl;
            } else {
                const double progress = (double)(i) / iterations;
                const double temp = TEMP_START * pow(TEMP_END / TEMP_START, progress);
                if (rng.next_double() < exp((new_score - best_score) / temp)) {
                    order = new_order;  // TODO: try swapping instead?
                    n_used = new_result.second;
                }
            }
        }
    }

    return recover_actions(best_order);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    cin >> hero;
    cin >> game;
    for (int i=0; i<game.num_monsters; ++i) {
        monster[i].id = i;
        cin >> monster[i];
    }

    cerr << "Finished reading input." << endl;

    calculate_dist();

    cerr << "Finished calculating dist." << endl;

    const vector<Action> answer = simulated_annealing(100, 10000);
    for (const Action& action: answer) {
        cout << action << '\n';
    }
}
