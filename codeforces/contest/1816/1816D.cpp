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
//	setIO("");
	int t;
	cin >> t;
	while (t--) {
		
		//edge between node i and x - i
		//for example n = 10 x = 6
		//add edges between nodes 1, 5  2, 4, 3, 3
		int n, resp;
		cin >> n;
		
		int arrangement[n + 1];
		int count = 0;
		for (int i = n - 1; i > 0; i -= 2) arrangement[i] = ++count;
		for (int i = ((n + 1) % 2) + 1; i <= n; i += 2) arrangement[i] = ++count;
/*		for (int i = 1; i <= n; i++) cout << arrangement[i] << " ";
		cout << "\n";*/
		
		cout << "+ " << n << "\n";
		cout.flush();
		cin >> resp;
		if (resp == -2) return 0;
		cout << "+ " << n + 1 << "\n";
		cout.flush();
		cin >> resp;
		if (resp == -2) return 0;
		
		int dist[n + 1], dist2[n + 1], ans1[n + 1], ans2[n + 1];
		memset(dist, 0, sizeof(dist));
		memset(dist2, 0, sizeof(dist2));
		pair<int, int> mxDist = {0, 0}; //distance value, index
		for (int i = 2; i <= n; i++) {
			cout << "? " << 1 << " " << i << "\n";
			cout.flush();
			cin >> resp;
			if (resp == -2) return 0;
			dist[i] = resp;
			if (dist[i] > mxDist.f) mxDist = {dist[i], i};
		}
		
		for (int i = 1; i <= n; i++) {
			if (i == mxDist.s) continue;
			cout << "? " << mxDist.s << " " << i << "\n";
			cout.flush();
			cin >> resp;
			if (resp == -2) return 0;
			dist2[i] = resp;
		}
		
		//the distance dist2[1] is the distance from either end of permutation
		for (int i = 1; i <= n; i++) {
			ans1[i] = arrangement[n - dist2[i]];
			ans2[i] = arrangement[dist2[i] + 1];
		}
		cout << "! ";
		for (int i = 1; i <= n; i++) cout << ans1[i] << " ";
		for (int i = 1; i <= n; i++) cout << ans2[i] << " ";
		cout << "\n";
		cout.flush();
		cin >> resp;
		if (resp == -2) return 0;
	}
}