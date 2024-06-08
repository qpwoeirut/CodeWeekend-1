#include <bits/stdc++.h>

#include "types.cpp"

using namespace std;

const int MAX_N = 3000;

int calculate_order_score(const vector<int>& order);

Monster monster[MAX_N];

int main() {
    cin.tie(0)->sync_with_stdio(0);

    Hero hero; cin >> hero;
    Game game; cin >> game;
    for (int i=0; i<game.num_monsters; ++i) {
        monster[i].id = i;
        cin >> monster[i];
    }
}
