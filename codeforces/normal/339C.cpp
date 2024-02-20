#include <bits/stdc++.h>
using namespace std;

//policy based ds
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
typedef tree<pair<int, int>, null_type, less<pair<int, int>>, rb_tree_tag, tree_order_statistics_node_update> ordered_multiset; //use index as pair second

//typing is hard
using ll = long long;
using db = long double;
using str = string;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
#define end1 "\n"
#define pb push_back
#define eb emplace_back
#define ins insert

//n-dimensional vector
template <typename T>
vector<T> ndvec(size_t size, T initialValue) {
	return vector<T>(size, initialValue);
}
template <typename T, typename... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//better math
ll cdiv(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll fdiv(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

//closest element in a set
template <typename T>
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
	template <typename T1, typename T2>
	size_t operator()(const pair<T1, T2> &x) const {
		uint64_t a = (*this)(x.fi);
		uint64_t b = (*this)(x.se);
		return a + 3 * b;
	}
};

//grid problems
vector<pii> stepsOrthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> stepsDiagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

#ifdef LOCAL
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.hpp"
#define dbg(...) debug_first(#__VA_ARGS__, sizeof(#__VA_ARGS__), __VA_ARGS__)
#else
#define dbg(...) 42
#endif

void solve() {

}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	string in;
	cin >> in;
	int m;
	cin >> m;
	vector<bool> weights(11);
	for(int i = 1; i <= 10; i++) {
		weights[i] = in[i - 1] - 48;
	}
	dbg(weights);

	vector<int> path(1000);
	vector<int> out;
	bool ok = false;
	unordered_map<int, unordered_map<int, unordered_map<int, bool>>> v;
	auto dfs = [&](int bal, int last, int amt, auto&& dfs) -> void {
		if (amt == m) {
			dbg(path);
			ok = true;
			out = vector<int>(path.begin(), path.end());
			return;
		}
		for(int i = 1; i <= 10; i++) {
			if (i == last || weights[i] == 0) continue;
			if (amt % 2 == 0) {
				if(bal - i >= 0 || v[bal - i][i][amt + 1]) {
					continue;
				}
				v[bal - i][i][amt + 1] = true;
				path[amt] = i;
				dfs(bal - i, i, amt + 1, dfs);
			} else {
				if (bal + i <= 0 || v[bal + i][i][amt + 1]) {
					continue;
				}
				v[bal + i][i][amt + 1] = true;
				path[amt] = i;
				dfs(bal + i, i, amt + 1, dfs);
			}
		}
	};
	dfs(0, 0, 0, dfs);

	if (ok) {
		cout << "YES\n";
		for(int i = 0; i < m; i++) {
			cout << out[i] << " \n"[i == m - 1];
		}
	} else {
		cout << "NO\n";
	}
}