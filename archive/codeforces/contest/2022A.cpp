#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using db = long double;

#define REP2(i, n) for (int i = 0; i < (n); i++)
#define REP3(i, a, b) for (int i = (a); i <= (b); i++)
#define PER2(i, n) for (int i = (n) - 1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &i : (a))

template <typename T> vector<T> ndvec(size_t size, T initial_value) {
    return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return vector<decltype(inner)>(head, inner);
}

constexpr ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

#ifdef LOCAL
#include "/home/rayb/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void solve() {
    int n, r;
    cin >> n >> r;
    int ans = 0;
    int extra = 0;
    int sum = 0;
    rep(i, n) {
        int a;
        cin >> a;
        sum += a;
        ans += a / 2;
        a %= 2;
        extra += a;
    }
    r -= ans;
    dbg(ans, r, extra);
    int best = 0;
    rep(i, 0, extra) {
        if ((r - i) * 2 >= extra-i) {
			dbg(i);
            best = max(best, i);
        }
    }
	dbg(best);
    cout << ans * 2 + best << '\n';
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

