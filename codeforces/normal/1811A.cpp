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
		int n;
		char d;
		string s;
		cin >> n >> d >> s;
		int i = 0;
		string out;
		while (s[i] >= d) {
			out += s[i];
			i++;
		}
		out += d;
		for (int j = i; j < n; j++) out += s[j];
		cout << out << "\n";
	}
}