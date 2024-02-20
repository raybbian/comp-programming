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

const int mxN = 1e5 + 5;

bool cat[mxN];
vector<int> adj[mxN];
int n, m, ans;

void dfs(int node, int prev, int catsEncountered) {
	if (cat[node]) {
		catsEncountered++;
	} else {
		catsEncountered = 0;
	}
	if (catsEncountered > m) return;
//	cout << "arrived at " << node << " with cats " << catsEncountered << "\n";
	if (adj[node].size() == 1 && node != 0) ans++;
	for (auto &i: adj[node]) {
		if (i == prev) continue;
		dfs(i, node, catsEncountered);
	}
}

int main() {
	setIO("");
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		cat[i] = (a == 1);
	}
	for (int i = 0; i < n - 1; i++) {
		int x, y;
		cin >> x >> y;
		x--, y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}
	ans = 0;
	dfs(0, 0, 0);
	cout << ans << "\n";
}