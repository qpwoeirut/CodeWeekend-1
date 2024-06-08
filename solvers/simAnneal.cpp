#include <bits/stdc++.h>
#include <json/value.h>
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


unordered_map<string, string> jDATA;

jDATA = {
    "hero": {
        "base_speed": 10,
        "base_power": 50,
        "base_range": 10,
        "level_speed_coeff": 50,
        "level_power_coeff": 50,
        "level_range_coeff": 50
    },
    "start_x": 62,
    "start_y": 53,
    "width": 100,
    "height": 100,
    "num_turns": 12,
    "monsters": [
        {
            "x": 30,
            "y": 24,
            "hp": 70,
            "gold": 206,
            "exp": 64
        },
        {
            "x": 81,
            "y": 56,
            "hp": 40,
            "gold": 170,
            "exp": 44
        },
        {
            "x": 32,
            "y": 50,
            "hp": 54,
            "gold": 170,
            "exp": 47
        },
        {
            "x": 63,
            "y": 46,
            "hp": 29,
            "gold": 141,
            "exp": 29
        },
        {
            "x": 50,
            "y": 24,
            "hp": 73,
            "gold": 183,
            "exp": 80
        },
        {
            "x": 40,
            "y": 14,
            "hp": 57,
            "gold": 157,
            "exp": 59
        },
        {
            "x": 40,
            "y": 34,
            "hp": 54,
            "gold": 154,
            "exp": 43
        }
    ]
}


signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    Json jDATA;
    loadJSON("data.json", jDATA);
}

