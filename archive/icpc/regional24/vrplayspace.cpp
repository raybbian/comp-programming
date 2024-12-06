#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

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

constexpr int popcnt(ll x) { return __builtin_popcountll(x); }
constexpr int bits(ll x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); } // floor(log_2(x))

constexpr long long c_div(long long a, long long b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr long long f_div(long long a, long long b) { return a / b - ((a ^ b) < 0 && a % b); }

#ifdef LOCAL
#include "/home/rayb/Projects/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void solve() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    if (s > t)
        swap(s, t);

    auto grid = ndvec<char>(n + 2, m + 2, '0');
    auto up = ndvec<int>(n + 2, m + 2, 0);
    rep(i, 1, n) rep(j, 1, m) { cin >> grid[i][j]; }
    rep(j, 0, m + 1) rep(i, 1, n) { up[i][j] = grid[i][j] == '0' ? i : up[i - 1][j]; }

    dbg(grid, up);
    ll ans = 0;
    rep(i, 1, n) {
        vector<pii> st; // mono-increasing
        int last_zero = 0;
        rep(j, 1, m) {
            if (grid[i + 1][j] == '0') {
                last_zero = j;
            }
            if (grid[i][j] == '0') {
                st.clear();
                continue;
            }

            int far_back = j;
            while (!st.empty() && st.back().first > i - up[i][j]) {
                far_back = min(far_back, st.back().second);
                st.pop_back();
            }
            if (i - up[i][j] >= s && (st.empty() || st.back().first < i - up[i][j]))
                st.emplace_back(i - up[i][j], far_back);

            // if right side has blocking
            if (up[i][j] < up[i][j + 1]) {
                dbg(i, j, st);
                auto start = partition_point(
                    st.begin(), st.end(), [&](pii rect) { return rect.first <= i - up[i][j + 1]; });

                // ignore all rects that don't contain last zero
                auto end = partition_point(start, st.end(),
                                           [&](pii rect) { return rect.second <= last_zero; });

                // ignore all rectangles closer than s
                end =
                    partition_point(start, end, [&](pii rect) { return j - rect.second + 1 >= s; });

                // temp ignore all rectangles closer than t
                auto less_t =
                    partition_point(start, end, [&](pii rect) { return j - rect.second + 1 >= t; });

                int far_maybe_tall = less_t - start;
                dbg(far_maybe_tall);

                // in st.begin(), less_t ALL rectangles are further than t away, and all have height
                // at least s
                ans += less_t - start;
                // from less_t to end, count number of rectangles of height at least t
                auto h_geq_t =
                    partition_point(less_t, end, [&](pii rect) { return rect.first < t; });
                ans += end - h_geq_t;

                int close_and_tall = end - h_geq_t;
                dbg(close_and_tall);
            }
        }
    }
    cout << ans << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
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
