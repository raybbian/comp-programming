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
    ll n, m;
    cin >> n >> m;
    if (n < m) {
        cout << -1 << "\n";
        return;
    }
    ll max_bits = n == 0 ? 0 : 63 - __builtin_clzll(n); // 0ind bits

    vector<ll> inc, dec, out; // stores indices where need decrease
    for (ll i = 0; i < 64; i++) {
        ll mask = (1LL << i);
        if ((n & mask) > (m & mask)) {
            dec.push_back(i);
        } else if ((n & mask) < (m & mask)) {
            inc.push_back(i);
        }
    }

    out.push_back(n);
    if ((!inc.empty()) && (dec.empty() || dec.back() < inc.back())) {
        cout << -1 << "\n";
        return;
    }

    if ((!inc.empty()) && dec.back() == max_bits) {
        bool ok = false;
        for (ll i = dec.back() - 1; i > inc.back(); i--) {
            // must be 1 in n between these two to swap
            ll mask = (1LL << i);
            if ((n & mask) != 0) {
                ok = true;
                break;
            }
        }
        if (!ok) {
            cout << -1 << "\n";
            return;
        }
    }

    ll y = (1LL << dec.back());
    for (auto &i : inc) {
        y += (1LL << i);
    }

    assert(y < n);
    assert((n ^ y) < n);
    n ^= y;
    out.push_back(n);
    dec.pop_back();

    while (!dec.empty()) {
        ll ind = dec.back();
        dec.pop_back();
        ll y = (1LL << ind);
        assert(y < n);
        assert((n ^ y) < n);
        n ^= y;
        out.push_back(n);
    }

    cout << out.size() - 1 << "\n";
    for (int i = 0; i < (int)out.size(); i++) {
        cout << out[i] << " \n"[i == (int)out.size() - 1];
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
