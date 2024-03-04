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
#define ln "\n"
#define pb push_back
#define pf push_front
#define eb emplace_back
#define ef emplace_front
#define ins insert

//n-dimensional vector
template <typename T>
vector<T> ndvec(size_t size, T initial_value) {
	return vector<T>(size, initial_value);
}
template <typename T, typename... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

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
vector<pii> steps_orthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> steps_diagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps_8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

#ifdef LOCAL
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.hpp"
#define dbg(...) debug_first(#__VA_ARGS__, sizeof(#__VA_ARGS__), __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

//insert snippets below:


void solve() {
	ll n;
	cin >> n;
	//1 + 4 + 7 + 10 + 13
	//1, 5, 12 <- starting index of sequences
	//(3(i - 1))(i)/2 + i -> denotes start of nth sequence
	//binary search for i less than n
	ll l = 0, r = 1e9;
	while(l < r) {
		ll mid = (l + r + 1) / 2;
		ll st_index = 3 * (mid - 1) * mid / 2 + mid;
		if (st_index <= n) {
			l = mid;
		} else {
			r = mid - 1;
		}
	}
	dbg(l);
	//digit is in sequence #l
	//if l is odd, then it is a 8, 9, 0 sequencce
	//if l is even, then it is a 1, 0, 9 sequence
	// length of sequence #l is l * 3 + 1
	ll len = l * 3 + 1;
	ll st_index = 3 * (l - 1) * l / 2 + l; 
	if (l % 2 == 1) {
		if (n == st_index) {
			cout << 8 << " ";
		} else if (n <= st_index + (len - 1) / 3) {
			cout << 9 << " ";
		} else {
			cout << 0 << " ";
		}
	} else {
		if (n == st_index) {
			cout << 1 << " ";
		} else if (n <= st_index + (len - 1) / 3) {
			cout << 0 << " ";
		} else {
			cout << 9 << " ";
		}
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) solve();
}