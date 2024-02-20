#ifdef LOCAL
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.h"
#define dbg(...) cerr << "(" << (#__VA_ARGS__) << "): ", debug(__VA_ARGS__)
#else
#include <bits/stdc++.h>
using namespace std;
#define dbg(...) 42
#endif

//typing is hard
using ll = long long;
using db = long double;
using str = string;

//ez pairs
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second

//ez containers
#define end1 "\n"
#define pb push_back
#define eb emplace_back
#define ins insert
#define sz(x) int((x).size())
#define beg(x) begin(x)
#define all(x) beg(x), end(x)

//n-dimensional vector
template <class T>
vector<T> ndvec(size_t size, T initialValue) {
	return vector<T>(size, initialValue);
}
template <class T, class... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//better math
ll cdiv(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll fdiv(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

//closest element in a set
template <class T>
auto closest_element(T &set, const typename T::value_type &value) -> decltype(set.begin()) {
	const auto it = set.lower_bound(value);
	if (it == set.begin()) return it;
	const auto prev_it = prev(it);
	return (it == set.end() || value - *prev_it <= *it - value) ? prev_it : it;
}

//custom hash
struct chash {
	static uint64_t splitmix64(uint64_t x) {
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}
	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
	template <class T, class U>
	size_t operator()(const pair<T, U> &x) const {
		uint64_t a = (*this)(x.fi);
		uint64_t b = (*this)(x.se);
		return a + 3 * b;
	}
};

//grid problems
vector<pii> stepsOrthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> stepsDiagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

void solve() {
	string s;
	cin >> s;
	vector<unordered_set<int>> freq(26);
	for(int i = 0; i < s.size(); i++) {
		if (i != 0) {
			freq[int(s[i - 1]) - 'a'].insert(int(s[i]) - 'a');
			freq[int(s[i]) - 'a'].insert(int(s[i - 1]) - 'a');
		}
		if (i != s.size() - 1) {
			freq[int(s[i]) - 'a'].insert(int(s[i + 1]) - 'a');
			freq[int(s[i + 1]) - 'a'].insert(int(s[i]) - 'a');
		}
	}
	dbg(freq);

	vector<bool> mk(26, false);
	auto dfs = [&](int node, int par, auto&& dfs) -> void {
		if (mk[node]) return;
		mk[node] = true;
		cout << char(node + 'a');
		for (auto &i: freq[node]) {
			if (i == par) continue;
			dfs(i, node, dfs);
		}
	};
	for(int i = 0; i < 26; i++) {
		if (freq[i].size() > 2) {
			cout << "NO\n";
			return;
		}
	}
	bool hasTwo = false;
	bool hasOne = false;
	for(int i = 0; i < 26; i++) {
		if (freq[i].size() == 2) hasTwo = true;
		if (freq[i].size() == 1) hasOne = true;
	}
	if (hasTwo && !hasOne) {
		cout << "NO\n";
		return;
	}

	cout << "YES\n";
	for(int i = 0; i < 26; i++) {
		if (freq[i].size() == 1) {
			dfs(i, -1, dfs);
			break;
		}
	}
	for(int i = 0; i < 26; i++) {
		if (mk[i]) continue;
		cout << char(i + 'a');
	}
	cout << "\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	for(int i = 0; i < t; i++) {
		solve();
	}
}