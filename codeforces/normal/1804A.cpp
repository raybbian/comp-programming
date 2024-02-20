#include <bits/stdc++.h>
using namespace std;

#define f first
#define s second
#define end1 "\n"

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		int a, b;
		cin >> a >> b;
		if (a < 0) a *= -1;
		if (b < 0) b *= -1;
		if (a > b) swap(a, b);
		int ans = 0;
		ans += a * 2;
		ans += (b - a) * 2;
		if (a < b) ans--;
		cout << ans << end1;
	}
}