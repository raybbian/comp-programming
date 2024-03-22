#include <bits/stdc++.h>
using namespace std;

// typing is hard
using ll = long long;
using db = long double;
using str = string;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
#define ln "\n"

// loops
#define REP2(i, n) for (int i = 0; i < (n); i++)
#define REP3(i, a, b) for (int i = (a); i <= (b); i++)
#define PER2(i, n) for (int i = (n)-1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &i : (a))

// n-dimensional vector
template <typename T> vector<T> ndvec(size_t size, T initial_value) {
    return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return vector<decltype(inner)>(head, inner);
}

// bitwise ops
constexpr int popcnt(int x) { return __builtin_popcount(x); }            // # of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x))

// better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

// check and assign max and min
template <typename T> bool chk_min(T &a, const T &b) { return b < a ? a = b, 1 : 0; }
template <typename T> bool chk_max(T &a, const T &b) { return a < b ? a = b, 1 : 0; }

#ifdef LOCAL
#include "/home/rayb/Documents/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void move(int ind) {
    cout << ind << ln;
    cout.flush();
}

int rd() {
    int z;
    cin >> z;
    if (z == -1)
        assert(false);
    return z;
}

void solve() {
    int n, sx, sy;
    cin >> n >> sx >> sy;
    vector<unordered_set<int>> freq(2);
    vector<int> parity(n + 1);
    rep(i, 1, n) {
        int x, y;
        cin >> x >> y;
        int par = (x + y) % 2;
        freq[par].insert(i);
        parity[i] = par;
    }
    int s_par = (sx + sy) % 2;
    if (s_par == 1) {
        swap(freq[0], freq[1]);
        rep(i, 1, n) { parity[i] = (parity[i] + 1) % 2; }
    }

    dbg(freq);
    // starting parity is 0
    // first player wins if he can make last par == st par
    int cnt = n;
    if ((int)freq[1].size() <= n / 2) {
        // take away all opposite par
        cout << "First" << ln;
        cout.flush();
        while (cnt > 0) {
            if (!freq[1].empty()) {
                auto it = freq[1].begin();
                move(*it);
                freq[1].erase(it);
            } else {
                auto it = freq[0].begin();
                move(*it);
                freq[0].erase(it);
            }
            cnt--;
            if (cnt == 0)
                break;
            int resp = rd();
            assert(freq[parity[resp]].count(resp));
            freq[parity[resp]].erase(resp);
            cnt--;
        }
    } else {
        cout << "Second" << ln;
        cout.flush();
        while (cnt > 0) {
            int resp = rd();
            assert(freq[parity[resp]].count(resp));
            freq[parity[resp]].erase(resp);
            cnt--;
            if (cnt == 0)
                break;
            if (!freq[0].empty()) {
                auto it = freq[0].begin();
                move(*it);
                freq[0].erase(it);
            } else {
                auto it = freq[1].begin();
                move(*it);
                freq[1].erase(it);
            }
            cnt--;
        }
    }
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
        solve();
}

/*
 * SANITY CHECKLIST:
 * 1. re-read the problem
 * 2. consider edge cases (n=1, overflow)
 * 3. come up with counter cases
 * 4. move on to the next problem
 * 5. re-read the problem
 * 6. re-read the problem upside-down
 * 7. re-read the problem backwords
 * 8. fucking re-read the problem
 */
