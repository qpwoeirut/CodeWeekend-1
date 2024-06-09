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

vector<Action> follow_path() {
    hero.reset();
    bitset<N> killed;

    int x = game.start_x, y = game.start_y;
    vector<Action> actions;
    for (pii p: PATH) {
        p.first += rng.next_int(-5, 5);
        p.second += rng.next_int(-5, 5);
        while (x != p.first || y != p.second) {
            for (int i=0; i<game.num_monsters; ++i) {
                if (!killed[i] && monster[i].hp < 10000 && square(monster[i].x - x) + square(monster[i].y - y) <= square(hero.get_range())) {
                    for (int pw=0; pw<monster[i].hp; pw += hero.get_power()) actions.emplace_back("attack", monster[i].id);
                    if ((int)actions.size() > game.num_turns) break;
                    hero.add_exp(monster[i].exp);
                    killed[i] = true;
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

        if (turns >= game.num_turns) return pii(gold, game.num_monsters - i);
    }
    return pii(gold, 0);
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

    const vector<Action> answer = follow_path();
    for (const Action& action: answer) {
        cout << action << '\n';
    }
}

