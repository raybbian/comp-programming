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
constexpr int bitsll(ll x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); }

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
    ll n;
    cin >> n;
    ll a = n, b = n;

    ll bits = __builtin_popcountll(n);
    // what if 2 bits? -> first, 1 1 win
    // what if 3 bits? -> go second, actually, bob split 1, 2, you go 1 1
    // what if 4 bits? -> first, 3 1 win
    // what if 5 bits? -> go second, bob goes 1 4 or 2 3, you win
    // what if 6 bits? -> first, 5 1 win

    if (bits % 2 == 0) {
        cout << "first\n";
        cout.flush();
    } else {
        cout << "second\n";
        cout.flush();
        cin >> a >> b;
        if ((a == 0 && b == 0) || (a == -1 && b == -1))
            return;
    }

    while (true) {
        // make choice between a, b
        // choose one with even bits, make both odd
        ll a_bit = __builtin_popcountll(a);
        ll b_bit = __builtin_popcountll(b);
        ll cur = -1;
        if (a_bit == 1 && b_bit == 1) {
            cout << "0 0\n";
            cout.flush();
        }

        if (a_bit % 2 == 0) {
            cur = a;
        } else {
            assert(b_bit % 2 == 0);
            cur = b;
        }
        // guarantee that cur has even bits, split into 1, odd

        ll mask = (1LL << bitsll(cur));
        cout << (cur & mask) << " " << (cur & ~mask) << "\n";
        cout.flush();

        cin >> a >> b;
        if ((a == 0 && b == 0) || (a == -1 && b == -1))
            return;
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
