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

template<typename Set>
auto closest_element(Set &set, const typename Set::value_type &value)
-> decltype(set.begin()) {
	const auto it = set.lower_bound(value);
	if (it == set.begin())
		return it;
	
	const auto prev_it = std::prev(it);
	return (it == set.end() || value - *prev_it <= *it - value) ? prev_it : it;
}

int main() {
	setIO("");
	int t;
	cin >> t;
	while (t--) {
		int n, m;
		cin >> n >> m;
		set<long long> lines;
		for (int i = 0; i < n; i++) {
			long long k;
			cin >> k;
			lines.insert(k);
		}
		for (int i = 0; i < m; i++) {
			long long a, b, c;
			cin >> a >> b >> c;
			
			long long consider = *closest_element(lines, b);
			
			if ((b - consider) * (b - consider) < 4 * a * c) {
				cout << "YES\n" << consider << "\n";
			} else {
				cout << "NO\n";
			}
		}
		//kx = ax^2 + bx + c
		//ax^2 + (b-k)x + c = 0
		//determinant is (b-k)^2 - 4ac
		//if 4ac > (b-k)^2 then there are no solutions, which is what we want
	}
}