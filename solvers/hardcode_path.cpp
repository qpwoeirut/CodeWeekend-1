#include <bits/stdc++.h>

#include "limits.cpp"
#include "rng.cpp"
#include "types.cpp"

using namespace std;
using pii = pair<int, int>;

// coordinates are (x, y)
#if TESTCASE == 36
// 26^2 + 20^2 < 33^2
const vector<pii> PATH({
    {95, 900}, {125, 950}, {155, 900},
    {220, 105}, {250,  50}, {280, 105},
    {345, 895}, {375, 950}, {405, 895},
    {470, 110}, {500,  50}, {530, 110},
    {595, 890}, {625, 950}, {655, 890},
    {720, 115}, {750,  50}, {780, 115},
    {845, 885}, {875, 951}, {905, 885},
    {950, 0}
});
#elif TESTCASE == 37
const vector<pii> PATH({
    {95, 900}, {125, 950}, {155, 900},
    {220, 105}, {250,  50}, {280, 105},
    {345, 895}, {375, 950}, {405, 895},
    {470, 110}, {500,  50}, {530, 110},
    {595, 890}, {625, 950}, {655, 890},
    {720, 115}, {750,  50}, {780, 115},
    {845, 885}, {875, 951}, {905, 885},
    {950, 0}
});
#else
fail with compile error
#endif

Hero hero;
Game game;
Monster monster[N];

inline int square(const int x) {
    return x * x;
}

Rng rng(chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count());

int calculate_fatigue(const bitset<N>& killed, const int x, const int y) {
    int fatigue = 0;
    for (int i=0; i<game.num_monsters; ++i) {
        if (!killed[i] && square(monster[i].x - x) + square(monster[i].y - y) <= square(monster[i].range)) fatigue += monster[i].attack;
    }
    return fatigue;
}

vector<Action> follow_path(const vector<pii>& path) {
    hero.reset();
    bitset<N> killed;

    int x = game.start_x, y = game.start_y;
    vector<Action> actions;
    int gold = 0;
    int fatigue = 0;
    int i = 0;
    while (i < (int)path.size()) {
        const pii p = path[i];
        for (int i=0; i<game.num_monsters; ++i) {
            if (!killed[i] && monster[i].hp < 10000 && square(monster[i].x - x) + square(monster[i].y - y) <= square(hero.get_range())) {
                const int added_turns = (monster[i].hp + hero.get_power() - 1) / hero.get_power();
                fatigue += (added_turns - 1) * calculate_fatigue(killed, x, y);
                for (int p=0; p<added_turns; ++p) actions.emplace_back("attack", monster[i].id);
                if ((int)actions.size() > game.num_turns) {
                    break;
                }
                gold += 1000 * monster[i].gold / (1000 + fatigue);
                hero.add_exp(monster[i].exp);
                killed[i] = true;
                fatigue += calculate_fatigue(killed, x, y);
                ++i;
                continue;
            }
        }

        double dx = p.first - x, dy = p.second - y;
        const int spd = x < 125 ? hero.get_speed() : min(hero.get_range(), hero.get_speed());
        const double mag = sqrt(min(square(spd) / (dx * dx + dy * dy), 1.0));

        dx *= mag;
        dy *= mag;

        const int nx0 = x + (int)(dx - 1);
        const int nx1 = x + (int)dx;
        const int nx2 = x + (int)(dx + 1);
        const int ny0 = y + (int)(dy - 1);
        const int ny1 = y + (int)dy;
        const int ny2 = y + (int)(dy + 1);

        const vector<pii> options({{nx0, ny0}, {nx0, ny1}, {nx0, ny2}, {nx1, ny0}, {nx1, ny1}, {nx1, ny2}, {nx2, ny0}, {nx2, ny1}, {nx2, ny2}});
        const pii nxt = *min_element(options.begin(), options.end(), [&p, &x, &y](const pii& a, const pii& b) {
            bool a_reachable = square(a.first - x) + square(a.second - y) <= square(hero.get_speed());
            bool b_reachable = square(b.first - x) + square(b.second - y) <= square(hero.get_speed());
            if (!a_reachable || !b_reachable) return a_reachable > b_reachable;

            const int a_rem = square(p.first - a.first) + square(p.second - a.second);
            const int b_rem = square(p.first - b.first) + square(p.second - b.second);
            return a_rem < b_rem;
        });

        x = nxt.first;
        y = nxt.second;
        actions.emplace_back("move", x, y);
        fatigue += calculate_fatigue(killed, x, y);

        while (x == path[i].first && y == path[i].second) ++i;

        if ((int)actions.size() >= game.num_turns) break;
    }
    while ((int)actions.size() > game.num_turns) actions.pop_back();

    cerr << "gold: " << gold << endl;
    return actions;
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

    sort(monster, monster + game.num_monsters, [](const Monster& a, const Monster& b) {
        return (long long)a.hp * b.exp < (long long)b.hp * a.exp;  // sort by hp / exp
    });

    cerr << "Finished reading input." << endl;

    const vector<Action> answer = follow_path(PATH);
    for (const Action& action: answer) {
        cout << action << '\n';
    }
}

