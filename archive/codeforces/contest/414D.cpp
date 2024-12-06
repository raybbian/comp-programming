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
#define PER2(i, n) for (int i = (n) - 1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &(i) : (a))

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
#include "/home/rayb/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

// binary search
template <typename T, typename U> T last_true(T l, T r, U f) {
    l--;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l + 1) / 2;
        f(mid) ? l = mid : r = mid - 1;
    }
    return l;
}
template <typename T, typename U> T first_true(T l, T r, U f) {
    r++;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l) / 2;
        f(mid) ? r = mid : l = mid + 1;
    }
    return l;
}

void solve() {
	int n;
    ll k, p;
    cin >> n >> k >> p;
	vector<vector<int>> adj(n + 1);
	rep(i, n - 1) {
		int a, b;
		cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	
	int max_depth = 0;
	vector<ll> depth(n + 1);
	auto dfs = [&](int node, int d, int par, auto&& dfs) -> void {
		depth[d]++;
		max_depth = max(max_depth, d);
		each(next, adj[node]) {
			if (next == par) continue;
			dfs(next, d + 1, node, dfs);
		}
	};
	dfs(1, 1, 0, dfs);
	vector<ll> ddepth(n + 1);
	rep(i, 1, n) { 
		ddepth[i] = (max_depth - i + 1) * depth[i] + ddepth[i - 1];
		depth[i] += depth[i - 1];
	}

	ll ans = 0;
	rep(lo, 2, max_depth) {
		auto ok = [&](int amt) -> int {
			//find highest level with nodes on
			auto ok2 = [&](int hhi) -> bool {
				ll lowb = depth[lo] - depth[hhi];
				return lowb < amt;
			};
			int hi = first_true(2, lo, ok2);

			ll amt_guarantee = depth[lo] - depth[hi];
			ll amt_extra = amt - amt_guarantee;
			if (amt_extra > depth[hi] - depth[hi - 1]) return false;

			ll amt_hold = amt_extra * (lo - hi) + (ddepth[lo] - ddepth[hi]) - (depth[lo] - depth[hi]) * (max_depth - lo + 1);
			dbg(amt_extra * (lo - hi), ddepth[lo] - ddepth[hi], depth[lo] - depth[hi], max_depth - lo);
			dbg(lo, hi, amt, depth, ddepth, amt_guarantee, amt_extra, amt_hold);
			assert(amt_extra + amt_guarantee == amt);
			assert(amt_extra <= depth[hi] - depth[hi - 1]);
			if (amt_hold > p) return false; // cost too much
			return true;
		};
		ans = max(ans, last_true(1LL, k, ok));
	}
	cout << ans << ln;
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
