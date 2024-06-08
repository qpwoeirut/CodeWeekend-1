#include <bits/stdc++.h>
//#pragma GCC optimize ("Ofast")
//#pragma GCC optimize("unroll-loops")
//#pragma GCC target ("avx2")
//#pragma GCC optimize("trapv")
using namespace std;
 
#define MOD 1000000007
#define MOD2 998244353
#define MAXN 2000005
#define MAXK 21
#define INF 2e9
#define INF2 2e18
#define ll long long
#define nl '\n'
#define pb push_back
#define pf push_front
#define fi first
#define se second
#define mp make_pair


typedef pair<int,int> pii; 
typedef pair<ll,ll> pll;

long long int_sqrt (long long x) {
    long long ans = 0;
    for (ll k = 1LL << 30; k != 0; k /= 2) {
        if ((ans + k) * (ans + k) <= x) {
            ans += k;
        }
    }
    return ans;
}



struct Monster {
    int x, y, hp, gold, exp, id;
    Monster(int x, int y, int hp, int gold, int exp, int id) : x(x), y(y), hp(hp), gold(gold), exp(exp), id(id) {}
};

struct Hero {
    int base_speed, base_power, base_range, level_speed_coeff, level_power_coeff, level_range_coeff;
} hero;

int start_x, start_y, width, height, num_turns, num_monsters;
vector<Monster> monsters;

void input(){
    cin >> hero.base_speed >> hero.base_power >> hero.base_range >> hero.level_speed_coeff >> hero.level_power_coeff >> hero.level_range_coeff;
    cin >> start_x >> start_y >> width >> height >> num_turns;
    int num_monsters; cin >> num_monsters;
    for (int i = 0; i < num_monsters; i++) {
        int x, y, hp, gold, exp; cin >> x >> y >> hp >> gold >> exp;
        monsters.push_back(Monster(x, y, hp, gold, exp, i));
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    input();
}

