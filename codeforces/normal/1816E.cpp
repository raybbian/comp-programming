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

int main() {
	setIO("");
	int t;
	cin >> t;
	while (t--) {
		int n, m;
		cin >> n >> m;
		vector<int> adj[n + 1];
		bool v[n + 1];
		memset(v, false, sizeof(v));
		for (int i = 0; i < m; i++) {
			int a, b;
			cin >> a >> b;
			if (a == 1) continue;
			adj[b].push_back(a);
		}
		queue<int> q;
		q.push(1);
		vector<vector<int>> buckets;
		string ans = "";
		while (!q.empty()) {
			int sz = q.size();
			vector<int> buck;
			for (int i = 0; i < sz; i++) {
				int num = q.front();
				q.pop();
				if (v[num]) {
					continue;
				}
				v[num] = true;
				
				buck.push_back(num);
				for (auto &j: adj[num]) {
					q.push(j);
				}
			}
			buckets.push_back(buck);
		}
		
		bool looseCond = false;
		for (int i = 1; i <= n; i++)
			if (!v[i]) looseCond = true;
		
		if (n > 1 && looseCond) { //if n > 1 and there are loose conditions
			cout << "INFINITE" << "\n";
		} else {
			cout << "FINITE" << "\n";
			string ans = "";
			int lengthOfAns = 0;
			unordered_set<int> print;
			for (int i = buckets.size() - 1; i >= 0; i--) {
				print.insert(i);
				for (auto &j: print) {
					if ((j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1))
						for (auto &k: buckets[j]) ans += to_string(k), ans += " ", lengthOfAns++;
				}
			}
			for (int i = 0; i < buckets.size(); i++) {
				print.erase(i);
				for (auto &j: print) {
					if ((j % 2 == 0 && i % 2 == 1) || (j % 2 == 1 && i % 2 == 0))
						for (auto &k: buckets[j]) ans += to_string(k), ans += " ", lengthOfAns++;
				}
			}
			cout << lengthOfAns << "\n";
			cout << ans << "\n";
		}
	}
}