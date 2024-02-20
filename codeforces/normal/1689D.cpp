#include <bits/stdc++.h>
using namespace std;

//policy based ds
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
typedef tree<pair<int, int>, null_type, less<pair<int, int>>, rb_tree_tag, tree_order_statistics_node_update> ordered_multiset; //use index as pair second

//typing is hard
using ll = long long;
using db = long double;
using str = string;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
#define end1 "\n"
#define pb push_back
#define eb emplace_back
#define ins insert

//n-dimensional vector
template <typename T>
vector<T> ndvec(size_t size, T initialValue) {
	return vector<T>(size, initialValue);
}
template <typename T, typename... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//better math
ll cdiv(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll fdiv(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

//closest element in a set
template <typename T>
auto closest_element(T &set, const typename T::value_type &value) -> decltype(set.begin()) {
	const auto it = set.lower_bound(value);
	if (it == set.begin()) return it;
	const auto prev_it = prev(it);
	return (it == set.end() || value - *prev_it <= *it - value) ? prev_it : it;
}

//custom hash
struct chash {
	static uint64_t splitmix64(uint64_t x) {
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
	template <typename T1, typename T2>
	size_t operator()(const pair<T1, T2> &x) const {
		uint64_t a = (*this)(x.fi);
		uint64_t b = (*this)(x.se);
		return a + 3 * b;
	}
};

//grid problems
vector<pii> stepsOrthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> stepsDiagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

#ifdef LOCAL
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.hpp"
#define dbg(...) debug_first(#__VA_ARGS__, sizeof(#__VA_ARGS__), __VA_ARGS__)
#else
#define dbg(...) 42
#endif

//insert snippets below:

int manhattan(pii a, pii b) {
	return abs(a.fi - b.fi) + abs(a.se - b.se);
}

void solve() {
	int n, m;
	cin >> n >> m;
	auto grid = ndvec<bool>(n, m, false);
	vector<pii> black;
	for(int i = 0; i < n; i++) {
		string row;
		cin >> row;
		for(int j = 0; j < m; j++) {
			if (row[j] == 'W') grid[i][j] = true;
			else black.eb(i, j);
		}
	}
	vector<pii> imp;
	sort(black.begin(), black.end(), [](pii &a, pii &b) {return a.fi - a.se < b.fi - b.se;});
	imp.pb(black.back());
	imp.pb(*black.begin());
	sort(black.begin(), black.end(), [](pii &a, pii &b) {return a.fi + a.se < b.fi + b.se;});
	imp.pb(black.back());
	imp.pb(*black.begin());

	int ans = INT_MAX;
	pii out;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			int mx = 0;
			for(auto &k : imp) {
				mx = max(mx, manhattan(k, {i, j}));
			}
			if (mx < ans) {
				out = {i + 1, j + 1};
				ans = mx;
			}
		}
	}
	dbg(out);
	cout << out.fi << " " << out.se << "\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}