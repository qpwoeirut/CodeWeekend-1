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



signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string s;cin >> s;
    cout << s << nl;
    return 0;
}

