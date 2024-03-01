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

void solve() {
    int n, m;
    cin >> n >> m;
    cout << "? 1 1\n";
    cout.flush();

    int resp;
    cin >> resp;
    //(r-1) + (c-1) = resp
    //
    // test when r = 1, c = 1
    pii aqry = {-1, -1};
    if (resp + 1 > m) {
        aqry = {1 + (resp + 1 - m), m};
    } else {
        aqry = {1, resp + 1};
    }
    cout << "? " << aqry.fi << " " << aqry.se << "\n";
    cout.flush();

    int a;
    cin >> a;
    pii apt = {-1, -1};
    if (a % 2 == 0 && aqry.fi + a / 2 <= n && aqry.se - a / 2 >= 1) {
        apt = {aqry.fi + a / 2, aqry.se - a / 2};
    }

    pii bqry = {-1, -1};
    if (resp + 1 > n) {
        bqry = {n, 1 + (resp + 1 - n)};
    } else {
        bqry = {resp + 1, 1};
    }
    cout << "? " << bqry.fi << " " << bqry.se << "\n";
    cout.flush();

    int b;
    cin >> b;
    pii bpt = {-1, -1};
    if (b % 2 == 0 && bqry.fi - b / 2 >= 1 && bqry.se + b / 2 <= m) {
        bpt = {bqry.fi - b / 2, bqry.se + b / 2};
    }

    dbg(apt, bpt);
    if (apt != pii{-1, -1} && bpt != pii{-1, -1}) {
        cout << "? " << apt.fi << " " << apt.se << "\n";
        cout.flush();
        int test;
        cin >> test;
        if (test == 0) {
            cout << "! " << apt.fi << " " << apt.se << "\n";
            cout.flush();
        } else {
            cout << "! " << bpt.fi << " " << bpt.se << "\n";
            cout.flush();
        }
    } else if (apt == pii{-1, -1}) {
        cout << "! " << bpt.fi << " " << bpt.se << "\n";
        cout.flush();
    } else if (bpt == pii{-1, -1}) {
        cout << "! " << apt.fi << " " << apt.se << "\n";
        cout.flush();
    } else {
        assert(false);
    }

    // check one of these points, if a or b not divisible by 2, return value from other
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
