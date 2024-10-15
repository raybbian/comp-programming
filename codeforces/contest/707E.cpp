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
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<ll>> garlands(k);
    vector<bool> active(k, true);
    vector<array<int, 4>> extreme(k, {INT_MAX, INT_MAX, INT_MIN, INT_MIN});
    vector<vector<array<int, 2>>> grid(n + 2, vector<array<int, 2>>(m + 2, {-1, -1}));
    rep(i, k) {
        int len;
        cin >> len;
        garlands[i] = vector<ll>(len + 1);
        rep(j, 1, len) {
            int r, c;
            cin >> r >> c >> garlands[i][j];
            garlands[i][j] += garlands[i][j - 1];
            grid[r][c] = {i, j}; // id, number
            extreme[i][0] = min(extreme[i][0], r);
            extreme[i][1] = min(extreme[i][1], c);
            extreme[i][2] = max(extreme[i][2], r);
            extreme[i][3] = max(extreme[i][3], c);
        }
    }
    int q;
    cin >> q;
    while (q--) {
        string type;
        cin >> type;
        if (type[0] == 'S') {
            int i;
            cin >> i;
            active[i - 1] = !active[i - 1];
        } else {
            int c1, r1, c2, r2;
            cin >> r1 >> c1 >> r2 >> c2;
            map<int, set<pair<int, int>>> segments;
            ll sum = 0;

            rep(r, r1, r2) {
                if (grid[r][c1][0] != -1 && active[grid[r][c1][0]] &&
                    grid[r][c1][0] == grid[r][c1 - 1][0] &&
                    abs(grid[r][c1][1] - grid[r][c1 - 1][1]) == 1) {
                    segments[grid[r][c1][0]].insert(
                        {grid[r][c1][1], (grid[r][c1][1] < grid[r][c1 - 1][1])});
                }
                if (grid[r][c2][0] != -1 && active[grid[r][c2][0]] &&
                    grid[r][c2][0] == grid[r][c2 + 1][0] &&
                    abs(grid[r][c2][1] - grid[r][c2 + 1][1]) == 1) {
                    segments[grid[r][c2][0]].insert(
                        {grid[r][c2][1], (grid[r][c2][1] < grid[r][c2 + 1][1])});
                }
            }

            rep(c, c1, c2) {
                if (grid[r1][c][0] != -1 && active[grid[r1][c][0]] &&
                    grid[r1][c][0] == grid[r1 - 1][c][0] &&
                    abs(grid[r1][c][1] - grid[r1 - 1][c][1]) == 1) {
                    segments[grid[r1][c][0]].insert(
                        {grid[r1][c][1], (grid[r1][c][1] < grid[r1 - 1][c][1])});
                }
                if (grid[r2][c][0] != -1 && active[grid[r2][c][0]] &&
                    grid[r2][c][0] == grid[r2 + 1][c][0] &&
                    abs(grid[r2][c][1] - grid[r2 + 1][c][1]) == 1) {
                    segments[grid[r2][c][0]].insert(
                        {grid[r2][c][1], (grid[r2][c][1] < grid[r2 + 1][c][1])});
                }
            }

            dbg(r1, c1, r2, c2, segments);
            for (auto &[id, segment_list] : segments) {
                assert(id != -1);
                if (!segment_list.empty() && segment_list.rbegin()->second == 0) {
                    // entered but did not exit
                    sum += garlands[id].back();
                }
                for (auto &[point, par] : segment_list) {
                    if (par == 0) { // entering
                        sum -= garlands[id][point - 1];
                    } else { // exiting
                        sum += garlands[id][point];
                    }
                }
            }

            dbg(sum);
            rep(i, k) {
                if (!active[i])
                    continue;
                if (extreme[i][0] >= r1 && extreme[i][1] >= c1 && extreme[i][2] <= r2 &&
                    extreme[i][3] <= c2) {
                    dbg(extreme[i]);
                    sum += garlands[i].back();
                }
            }
            cout << sum << '\n';
        }
    }
}

int main() {
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
