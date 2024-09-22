#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string.h>
#include <vector>

using namespace std;

#define MOD 1000000007
#define fir first
#define sec second
#define int long long
#define pushf push_front
#define pushb push_back
#define popf pop_front
#define popb pop_back
#define mp make_pair

#define FOR(i, s, e) for (int i = s; i < e; i++)
#define REP(i, n) FOR(i, 0, n)

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<bool> vb;
typedef vector<char> vc;
typedef vector<string> vs;
typedef vector<pii> vpii;
typedef vector<pll> vpll;
typedef vector<vi> vvi;
typedef vector<vll> vvll;
typedef vector<vb> vvb;
typedef vector<vc> vvc;
typedef vector<vpii> vvpii;
typedef queue<int> qi;
typedef queue<ll> qll;
typedef queue<pii> qpii;
typedef queue<pll> qpll;
typedef deque<int> dqi;
typedef deque<ll> dqll;
typedef deque<pii> dqpii;
typedef deque<pll> dqpll;
typedef priority_queue<int> pqi;
typedef priority_queue<ll> pqll;
typedef priority_queue<pii> pqpii;
typedef priority_queue<pll> pqpll;
typedef priority_queue<int, vi, greater<int>> r_pqi;
typedef priority_queue<ll, vll, greater<ll>> r_pqll;
typedef priority_queue<pii, vpii, greater<pii>> r_pqpii;
typedef priority_queue<pll, vpll, greater<pll>> r_pqpll;

// bruh so hard

const ll MX = 9e18;
int n;
ll DP[21][11];

signed main() {
    ios sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // precompute :)
    DP[0][0] = 1;
    for (int i = 1; i <= 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (j == 4) {
                continue;
            }
            for (int k = 0; k < 10; k++) {
                if (j == 1 && k == 3) {
                    continue;
                }
                DP[i][j] = min(MX, DP[i][j] + DP[i - 1][k]);
            }
        }
    }
    cin >> n;
    for (int i = 0; i < n; i++) {
        int t;
        ll v;
        cin >> t >> v;
        if (t == 1) {
            // Lucky to Normal
            ll c = v;
            bool valid = true;
            while (c) {
                if (c % 100 == 13 || c % 10 == 4) {
                    valid = false;
                    break;
                }
                c /= 10;
            }
            if (valid) {
                string s = to_string(v);
                ll ans = 0;

                for (int i = s.size(); i; i--) {
                    for (int j = 0; j < s[s.size() - i] - '0'; j++) {
                        if (i < s.size() && s[s.size() - i - 1] == '1' && j == 3)
                            continue;
                        ans += DP[i][j];
                    }
                }
                cout << ans << endl;
            } else {
                cout << "-1\n";
            }
        } else {
            // Normal to Lucky
            ll ans = 0;
            int prev = 0;
            for (int i = 20; i; i--) {
                for (int j = 0; j < 10; j++) {
                    if (prev == 1 && j == 3)
                        continue;

                    if (DP[i][j] > v) {
                        ll mult = 1;

                        for (int k = 0; k + 1 < i; k++) {
                            mult *= 10;
                        }

                        ans += mult * j;

                        prev = j;
                        break;
                    } else {
                        v -= DP[i][j];
                    }
                }
            }
            cout << ans << endl;
        }
    }
    return 0;
}
