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

bool cmp(const pair<long long, long long> &a, const pair<long long, long long> &b) {
	if (a.s == b.s) return a.f < b.f;
	return a.s > b.s;
}

priority_queue<long long, vector<long long>, greater<long long>> pq;

int main() {
	setIO("");
	int n;
	long long health = 0;
	cin >> n;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		pq.push(a);
		health += a;
		
		while (health < 0) {
			health -= pq.top();
			pq.pop();
		}
	}
	
	cout << pq.size() << "\n";
}