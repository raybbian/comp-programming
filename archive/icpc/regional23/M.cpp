#include <bits/stdc++.h>
using namespace std;

#define vt vector

int main() {
	int x; cin>>x;
	vt<int> v;
	while (x) {
		v.push_back(x%10);
		x/=10;
	}
	reverse(v.begin(), v.end());
	for (int i=0; i<v.size(); i++) {
		if (v[i]!=i+1) {
			cout << -1 << endl;
			return 0;
		}
	}
	cout << v.size() << endl;
}
