#include <bits/stdc++.h>
using namespace std;

#define f first
#define s second

void setIO(string name) {
	cin.tie(nullptr)->sync_with_stdio(false);
	if (name.length() > 0) {
		freopen((name + ".in").c_str(), "r", stdin);
		freopen((name + ".out").c_str(), "w", stdout);
	}
}

bool cmp(const tuple<int, int, int> &a, const tuple<int, int, int> &b) {
	if (get<2>(a) == get<2>(b)) {
		if (get<0>(a) == get<0>(b)) return get<1>(a) < get<1>(b);
		return get<0>(a) < get<0>(b);
	}
	return get<2>(a) < get<2>(b);
}

set<tuple<int, int, int>, decltype(cmp) *> adj(cmp);

struct hopcroft { //0 based
	int n, m; // size of each side
	int ans;
	vector<int> mu, mv; // u is matched with mu[u] and v with mv[v], -1 if not matched
	vector<vector<int> > adj;
	vector<int> layer;
	hopcroft(int n, int m) :
			n(n), m(m), ans(0),
			mu(n, -1), mv(m, -1),
			adj(n), layer(n) {}
	void add_edge(int u, int v) {
		adj[u].push_back(v);
		if (mu[u] == -1 and mv[v] == -1)
			ans++, mu[u] = v, mv[v] = u;
	}
	void bfs() {
		queue<int> q;
		for (int u = 0; u < n; u++) {
			if (mu[u] == -1) q.push(u), layer[u] = 0;
			else layer[u] = -1;
		}
		while (!q.empty()) {
			int u = q.front();
			q.pop();
			for (auto v: adj[u])
				if (mv[v] != -1 and layer[mv[v]] == -1) {
					layer[mv[v]] = layer[u] + 1;
					q.push(mv[v]);
				}
		}
	}
	bool dfs(int u) {
		for (auto v: adj[u])
			if (mv[v] == -1) {
				mu[u] = v, mv[v] = u;
				return (1);
			}
		for (auto v: adj[u])
			if (layer[mv[v]] == layer[u] + 1 and dfs(mv[v])) {
				mu[u] = v, mv[v] = u;
				return (1);
			}
		return (0);
	}
	int solve() { // O( sqrt(V) * E )
		while (true) {
			bfs();
			int augment = 0;
			for (int u = 0; u < n; u++)
				if (mu[u] == -1)
					augment += dfs(u);
			if (!augment)
				break;
			ans += augment;
		}
		return (ans);
	}
};

bool works(int n, int minDays) {
	hopcroft h(n, n);
	for (auto it = adj.begin(); it != adj.upper_bound({INT_MAX, INT_MAX, minDays}); it++) {
		h.add_edge(get<0>(*it) - 1, get<1>(*it) - 1);
	}
	h.solve();
	for (int i = 0; i < n; i++) {
		if (h.mu[i] == -1 || h.mv[i] == -1) return false;
	}
	return true;
}

int main() {
	setIO("");
	int n, m;
	cin >> n >> m;
	int maxD = 0;
	for (int i = 0; i < m; i++) {
		int u, v, d;
		cin >> u >> v >> d;
		adj.insert({u, v, d});
		maxD = max(maxD, d);
	}
	
	int l = 0, r = maxD + 1;
	while (l < r) {
		int mid = (l + r) / 2;
//		cout << "MIN DAYS: " << mid << "\n";
		if (works(n, mid)) {
//			cout << "works" << "\n";
			r = mid;
		} else {
			l = mid + 1;
//			cout << "doesn't work" << "\n";
		}
	}
	
	cout << ((r > maxD) ? -1 : r) << "\n";
}

/*
 * need 1 2 from 1 3 need a 3 from a n need b
 *
 *
 * add edges to used
 * {1, 2}
 * {2, 3}
 * {3, 3}
 *
 * new edge swapping in
 * {2, 1}
 * */