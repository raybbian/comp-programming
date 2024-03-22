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

class Comp {
public:
	bool operator()(const pair<int, int> &a, const pair<int, int> &b) const {
		if (a.f == b.f) return a.s < b.s;
		return a.f > b.f;
	}
};

template<class T>
class BIT {
private:
	int size;
	vector<T> bit;
	vector<T> arr;

public:
	BIT(int size) : size(size), bit(size + 1), arr(size) {}
	
	/** Sets the value at index ind to val. */
	void set(int ind, int val) { add(ind, val - arr[ind]); }
	
	/** Adds val to the element at index ind. */
	void add(int ind, int val) {
		arr[ind] += val;
		ind++;
		for (; ind <= size; ind += ind & -ind) { bit[ind] += val; }
	}
	
	/** @return The sum of all values in [0, ind]. */
	T pref_sum(int ind) {
		ind++;
		T total = 0;
		for (; ind > 0; ind -= ind & -ind) { total += bit[ind]; }
		return total;
	}
};

//sight level, seat order in which should enter
//stores as row, column

int main() {
	setIO("");
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		int n, m;
		cin >> n >> m;
		unordered_map<int, priority_queue<pair<int, int>, vector<pair<int, int>>, Comp>> seats;
		unordered_map<int, vector<int>> order;
		vector<int> sight;
		map<int, int> sortedSight;
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < m; k++) {
				int a;
				cin >> a;
				sight.push_back(a);
				sortedSight[a]++;
				// for all of same sight level that go between two rows, fill in the end of the first row first?
			}
		}
		int r = 1, c = 1;
		for (auto &j: sortedSight) {
			for (int k = 0; k < j.s; k++) {
				seats[j.f].push({r, c});
				if (c == m) {
					r++;
					c = 1;
				} else {
					c++;
				}
			}
		}
		
		for (auto &l: sight) {
			order[seats[l].top().f].push_back(seats[l].top().s);
			seats[l].pop();
		}
		
		int ans = 0;
		for (auto &j: order) {
			BIT<int> bit(m);
			for (int k = 0; k < m; k++) {
				bit.set(k, 0);
			}
			for (auto &k: j.s) {
				ans += bit.pref_sum(k - 2);
				bit.add(k - 1, 1);
//				cout << "{" << j.f << " " << k << "}, ";
			}
		}
		cout << ans << "\n";
	}
}