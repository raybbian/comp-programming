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

const int mxN = 1e5 + 1;
vector<int> adj[mxN];
int fir[mxN], sec[mxN], ans[mxN];

void dfs1(int node, int prev) {
	for (auto &i: adj[node]) {
		if (i != prev) {
			dfs1(i, node);
			if (fir[i] + 1 > fir[node]) {
				sec[node] = fir[node];
				fir[node] = fir[i] + 1;
			} else if (fir[i] + 1 > sec[node]) {
				sec[node] = fir[i] + 1;
			}
		}
	}
}

void dfs2(int node, int prev, int dist) {
	ans[node] = max(dist, fir[node]);
	for (auto &i: adj[node]) {
		if (i != prev) {
			if (fir[i] + 1 == fir[node])
				dfs2(i, node, max(dist, sec[node]) + 1);
			else dfs2(i, node, ans[node] + 1);
		}
	}
}

unordered_map<int, vector<int>> dists;

int main() {
	setIO("");
	int n;
	cin >> n;
	for (int i = 0; i < n - 1; i++) {
		int u, v;
		cin >> u >> v;
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs1(0, -1);
	dfs2(0, -1, 0);
	
	for (int i = 0; i < n; i++) {
		dists[ans[i]].push_back(i);
	}
	
	int ans = n;
	bool first = true;
	deque<int> out;
	for (int i = n; i > 0; i--) {
		if (dists.count(i)) {
			if (first) {
				ans++;
				first = false;
			}
			for (auto &j: dists[i]) {
				ans--;
			}
		}
		out.push_front(ans);
	}
	for (auto &i: out) {
		cout << i << " ";
	}
	cout << "\n";
	
}