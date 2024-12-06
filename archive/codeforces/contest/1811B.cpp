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

//0 is x / 2, x / 2 + 1 --- y / 2, y / 2 + 1;
//

int getRing(int x1, int y1) {
	return max(abs(x1), abs(y1));
}

int main() {
	setIO("");
	int t;
	cin >> t;
	while (t--) {
		int n, x1, x2, y1, y2;
		cin >> n >> x1 >> y1 >> x2 >> y2;
		x1 -= n / 2, x2 -= n / 2, y1 -= n / 2, y2 -= n / 2; //gets which circle they are in?
		if (x1 <= 0) x1--;
		if (x2 <= 0) x2--;
		if (y1 <= 0) y1--;
		if (y2 <= 0) y2--;

//		cout << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
		cout << abs(getRing(x1, y1) - getRing(x2, y2)) << "\n";
	}
}