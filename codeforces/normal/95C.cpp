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
#define pf push_front
#define eb emplace_back
#define ef emplace_front
#define ins insert

//n-dimensional vector
template <typename T>
vector<T> ndvec(size_t size, T initial_value) {
	return vector<T>(size, initial_value);
}
template <typename T, typename... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

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
vector<pii> steps_orthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> steps_diagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps_8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

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
	int n, m, x, y;
	cin >> n >> m >> x >> y;
	vector<vector<pair<int, ll>>> roads(n + 1);
	for(int i = 0; i < m; i++) {
		int u, v;
		ll w;
		cin >> u >> v >> w;
		roads[u].eb(v, w);
		roads[v].eb(u, w);
	}
	vector<vector<int>> adj(n + 1);
	vector<ll> cost(n + 1);
	for(int i = 1; i <= n; i++) {
		//dijkstra from each junction to find shortest paths from junction
		//unite all junctions under max length to dsu
		ll max_dist;
		cin >> max_dist >> cost[i];
		priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
		vector<ll> dist(n + 1, LLONG_MAX);
		pq.push({0, i});
		dist[i] = 0;
		while(!pq.empty()) {
			int junc;
			ll d;
			tie(d, junc) = pq.top();
			pq.pop();
			if (d != dist[junc]) continue;
			for(auto &i: roads[junc]) {
				if (d + i.se < dist[i.fi] && d + i.se <= max_dist) {
					dist[i.fi] = d + i.second;
					pq.push({dist[i.fi], i.fi});
				}
			}
		}
		dbg(dist);
		for(int j = 1; j <= n; j++) {
			if (dist[j] != LLONG_MAX) {
				adj[i].pb(j);
			}
		}
	}
	dbg(adj);

	vector<bool> v(n + 1, false);
	vector<ll> dist(n + 1, LLONG_MAX);
	priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
	pq.push({0, x});
	dist[x] = 0;
	while(!pq.empty()) {
		int junc;
		ll c_cost;
		tie(c_cost, junc) = pq.top();
		pq.pop();
		if (c_cost != dist[junc]) continue;
		for(auto &i: adj[junc]) {
			if (c_cost + cost[junc] < dist[i]) {
				dist[i] = c_cost + cost[junc];
				pq.push({dist[i], i});
			}
		}
	}
	if (dist[y] == LLONG_MAX) dist[y] = -1;
	cout << dist[y] << end1;
}