#include <bits/stdc++.h>

#include "limits.cpp"
#include "rng.cpp"
#include "types.cpp"

using namespace std;
using pii = pair<int, int>;

// Parameters to tune
constexpr const double TEMP_START = 10;
constexpr const double TEMP_END = 0.001;
constexpr const double REVERSE_CHANCE = 0.5;  // Reverse a subarray of the order.
constexpr const double SHIFT_CHANCE = 0.5;  // Swap the locations of two subarrays in the order.

static_assert(REVERSE_CHANCE + SHIFT_CHANCE == 1.0);

Hero hero;
Game game;
Monster monster[N];

inline int square(const int x) {
    return x * x;
}

// TODO: try to consteval this
int dist[MAX_LEVEL + 1][H][W];
vector<pii> reachable[MAX_LEVEL + 1];
void calculate_dist() {
    hero.reset();
    for (int level = 0; level <= MAX_LEVEL; ++level) {
        for (int x=0; x<W; ++x) {
            for (int y=0; y<H; ++y) {
                dist[level][y][x] = W * H;
            }
        }

        for (int x=-W + 1; x < W; ++x) {
            for (int y=-H + 1; y < H; ++y) {
                if (x * x + y * y <= square(hero.get_speed(level))) reachable[level].emplace_back(y, x);
            }
        }
        reachable[level].shrink_to_fit();

        //cerr << "level " << level << ' ' << reachable[level].size() << endl;

        // Sort by descending distance because why not.
        sort(reachable[level].begin(), reachable[level].end(), [](const pii& a, const pii& b) {
            return square(a.first) + square(a.second) > square(b.first) + square(b.second);
        });

        queue<pii> q;
        for (int x=0; x<W; ++x) {
            for (int y=0; y<H; ++y) {
                if (x * x + y * y <= square(hero.get_range(level))) {
                    dist[level][y][x] = 0;
                    q.emplace(y, x);
                }
            }
        }
        while (q.size() > 0) {
            const pii cur = q.front(); q.pop();
            
            if ((cur.first + 1 < H && dist[level][cur.first][cur.second] != dist[level][cur.first + 1][cur.second])
                 || (cur.second + 1 < W && dist[level][cur.first][cur.second] != dist[level][cur.first][cur.second + 1])) {
                for (const pii& d: reachable[level]) {
                    if (d.first >= 0 && d.second >= 0 && cur.first + d.first < H && cur.second + d.second < W && dist[level][cur.first + d.first][cur.second + d.second] > dist[level][cur.first][cur.second] + 1) {
                        dist[level][cur.first + d.first][cur.second + d.second] = dist[level][cur.first][cur.second] + 1;
                        q.emplace(cur.first + d.first, cur.second + d.second);
                    }
                }
            }
        }
    }
}

pii calculate_order_score(const array<int, N>& order) {
    hero.reset();

    int x = game.start_x, y = game.start_y;
    int turns = 0;
    int gold = 0;
    int i = 0;
    while (i < game.num_monsters) {
        const int dx = monster[order[i]].x - x, dy = monster[order[i]].y - y;
        if (dx * dx + dy * dy <= hero.get_range() * hero.get_range()) {
            turns += (monster[order[i]].hp + hero.get_power() - 1) / hero.get_power();
            if (turns > game.num_turns) return pii(gold, game.num_monsters - i);

            gold += monster[order[i]].gold;
            hero.add_exp(monster[order[i]].exp);
            ++i;
            continue;
        }
        const pii best = *min_element(reachable[hero.level].begin(), reachable[hero.level].end(), [&i, &order, &x, &y](const pii& a, const pii& b) {
            bool a_ok = 0 <= x + a.second && x + a.second < W && 0 <= y + b.first && y + b.first < H;
            bool b_ok = 0 <= x + b.second && x + b.second < W && 0 <= y + b.first && y + b.first < H;
            if (a_ok != b_ok) return a_ok > b_ok;
            int new_exp = 0;
            for (int j=i; j<game.num_monsters; ++j) {
                const int level = hero.get_prosp_level(new_exp);
                const int dx_a = abs(monster[order[j]].x - (x + a.second));
                const int dy_a = abs(monster[order[j]].y - (y + a.first));
                const int dx_b = abs(monster[order[j]].x - (x + b.second));
                const int dy_b = abs(monster[order[j]].y - (y + b.first));

                if (dist[level][dy_a][dx_a] != dist[level][dy_b][dx_b]) {
                    return dist[level][dy_a][dx_a] < dist[level][dy_b][dx_b];
                }

                new_exp += monster[order[j]].exp;
            }
            return false;
        });

        x += best.second;
        y += best.first;
        ++turns;

        if (turns >= game.num_turns) return pii(gold, game.num_monsters - i);
    }
    return pii(gold, 0);
}

vector<Action> recover_actions(const array<int, N>& order) {
    hero.reset();

    int x = game.start_x, y = game.start_y;
    vector<Action> actions;
    int gold = 0;
    int i = 0;
    while (i < game.num_monsters) {
        const int dx = monster[order[i]].x - x, dy = monster[order[i]].y - y;
        if (dx * dx + dy * dy <= hero.get_range() * hero.get_range()) {
            for (int p=0; p<monster[order[i]].hp; p += hero.get_power()) actions.emplace_back("attack", monster[order[i]].id);
            if ((int)actions.size() >= game.num_turns) {
                while ((int)actions.size() > game.num_turns) actions.pop_back();
                return actions;
            }

            gold += monster[order[i]].gold;
            hero.add_exp(monster[order[i]].exp);
            ++i;
            continue;
        }
        const pii best = *min_element(reachable[hero.level].begin(), reachable[hero.level].end(), [&i, &order, &x, &y](const pii& a, const pii& b) {
            int new_exp = 0;
            for (int j=i; j<game.num_monsters; ++j) {
                const int level = hero.get_prosp_level(new_exp);
                const int dx_a = abs(monster[order[j]].x - (x + a.second));
                const int dy_a = abs(monster[order[j]].y - (y + a.first));
                const int dx_b = abs(monster[order[j]].x - (x + b.second));
                const int dy_b = abs(monster[order[j]].y - (y + b.first));

                if (dist[level][dy_a][dx_a] != dist[level][dy_b][dx_b]) {
                    return dist[level][dy_a][dx_a] < dist[level][dy_b][dx_b];
                }

                new_exp += monster[order[j]].exp;
            }
            return false;
        });

        x += best.second;
        y += best.first;
        actions.emplace_back("move", x, y);

        if ((int)actions.size() >= game.num_turns) return actions;
    }
    return actions;
}

Rng rng(8);
array<int, N> mutate(array<int, N> order, const int n_used) {  // pass in a copy to mutate
    int idx1 = rng.next_int(n_used);
    int idx2 = rng.next_int(game.num_monsters);
    while (idx1 == idx2) idx2 = rng.next_int(game.num_monsters);
    if (idx1 > idx2) swap(idx1, idx2);

    const double choice = rng.next_double();
    if (choice < REVERSE_CHANCE) {
        for (int i=0; i+i<idx2 - idx1; ++i) {
            swap(order[idx1 + i], order[idx2 - i]);
        }
    } else {
        const int len = rng.next_int(min(idx2 - idx1, game.num_monsters - idx2)) + 1;
        for (int i=0; i<len; ++i) {
            swap(order[idx1 + i], order[idx2 + i]);
        }
    }
    return order;
}

//vector<Action> simulated_annealing(const int attempts, const int iterations) {
vector<Action> simulated_annealing(const int attempts, const chrono::milliseconds time_limit) {
    int best_score = 0;
    array<int, N> best_order;

    for (int attempt=0; attempt<attempts; ++attempt) {
        cerr << "Starting attempt " << attempt + 1 << " of " << attempts << endl;
        const chrono::time_point start_time = chrono::steady_clock::now();

        int cur_score = 0;
        array<int, N> cur_order, order;
        for (int i=0; i<game.num_monsters; ++i) order[i] = i;

        int n_used = 1;
        //for (int i=0; i<iterations; ++i) {
        while (chrono::steady_clock::now() - start_time < time_limit) {
            // TODO: consider early termination if no new best is found after a while, signal handling for killing
            const array<int, N> new_order = mutate(order, n_used);

            const pii new_result = calculate_order_score(new_order);
            const int new_score = new_result.first;

            if (cur_score < new_score) {
                cur_score = new_score;
                cur_order = new_order;
                order = new_order;  // TODO: try swapping instead?
                n_used = new_result.second;

                //cerr << "New current score: " << cur_score << endl;
            } else {
                //const double progress = (double)(i) / iterations;
                const double progress = (chrono::steady_clock::now() - start_time) / time_limit;
                const double temp = TEMP_START * pow(TEMP_END / TEMP_START, progress);
                if (rng.next_double() < exp((new_score - cur_score) / temp)) {
                    order = new_order;  // TODO: try swapping instead?
                    n_used = new_result.second;
                }
            }
        }

        if (best_score < cur_score) {
            best_score = cur_score;
            best_order = cur_order;
            cerr << "New best score: " << best_score << endl;
        }
    }

    return recover_actions(best_order);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    cin >> hero;
    cin >> game;
    assert(game.num_monsters <= N);
    assert(game.width <= W);
    assert(game.height <= H);
    for (int i=0; i<game.num_monsters; ++i) {
        monster[i].id = i;
        cin >> monster[i];
    }
    sort(monster, monster + game.num_monsters, [](const Monster& a, const Monster& b) {
        if (a.gold != b.gold) return a.gold > b.gold;
        if (a.exp != b.exp) return a.exp > b.exp;
        return a.id < b.id;
    });
    while (game.num_monsters > 1 && monster[game.num_monsters - 1].exp == 1 && monster[game.num_monsters - 1].gold == 1) --game.num_monsters;

    cerr << "Finished reading input." << endl;

    calculate_dist();

    cerr << "Finished calculating dist." << endl;

    const vector<Action> answer = simulated_annealing(20, 300s);
    for (const Action& action: answer) {
        cout << action << '\n';
    }
}

