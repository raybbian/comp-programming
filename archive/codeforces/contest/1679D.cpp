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


void solve() {

}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int n, m;
	ll k;
	cin >> n >> m >> k;
	vector<int> a(n);
	vector<vector<int>> adj(n);
	vector<vector<int>> r_adj(n);
	for(int i = 0; i < n; i++) {
		cin >> a[i];
	}
	for(int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		u--, v--;
		adj[u].pb(v);
		r_adj[v].pb(u);
	}
	dbg(n, m, k, a, adj);

	auto longest_path = [&](ll max_cost) -> ll {
		queue<int> q;
		vector<int> dist(n, 0);
		vector<int> toposort;
		vector<int> _in_degree(n, 0);
		for(int i = 0; i < n; i++) {
			if (a[i] > max_cost) continue;
			for(auto &j: adj[i]) {
				_in_degree[j]++;
			}
		}
		for(int i = 0; i < n; i++) {
			if (_in_degree[i] == 0 && a[i] <= max_cost) {
				q.push(i);
			}
		}
		
		while(!q.empty()) {
			int node = q.front();
			toposort.pb(node);
			q.pop();
			
			for(int &i: adj[node]) {
				if (a[i] > max_cost) continue;
				_in_degree[i]--;
				if (_in_degree[i] == 0) q.push(i);
			}

			for(int &i : r_adj[node]) {
				if (a[i] > max_cost) continue;
				dist[node] = max(dist[node], dist[i] + 1);
			}
		}
		int n_count = 0;
		for(int i = 0; i < n; i++) {
			if (a[i] <= max_cost) n_count++;
		}
		if (toposort.size() != n_count) return LLONG_MAX;
		return *max_element(dist.begin(), dist.end());
	};

	ll l = 0, r = 2e9;
	while(l < r) {
		ll mid = l + (r - l) / 2;
		if (longest_path(mid) >= k - 1) {
			r = mid;
		} else {
			l = mid + 1;
		}
	}
	if (k == 1) cout << *min_element(a.begin(), a.end()) << end1;
	else if (l == 2e9) cout << -1 << end1;
	else cout << l << end1;
}