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
#define dbg(...) debug_first(#__VA_ARGS__, sizeof(#__VA_ARGS__), __VA_ARGS__)
#else
#define dbg(...) 42
#endif

//insert snippets below:


void solve() {

}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int n, m;
	cin >> n >> m;
	//if head needs to go to two of them, it does not matter which one they go first
	//if one head is just closer, it should take care of it
	//no head should ever cross another head?
	vector<ll> heads;
	for(int i = 0; i < n; i++) {
		ll h;
		cin >> h;
		heads.pb(h);
	}
	vector<ll> _read;
	for(int i = 0; i < m; i++) {
		ll p;
		cin >> p;
		_read.pb(p);
	}
	dbg(heads, _read);
	deque<ll> read;
	auto ok = [&](ll max_time) -> bool {
		int head_pointer = 0;
		while(!read.empty()) {
			ll to_read = read.front();
			read.pop_front();
			if (head_pointer == n) return false;
			while(abs(heads[head_pointer] - to_read) > max_time) {
				head_pointer++;
				if (head_pointer == n) return false;
			}
			if (heads[head_pointer] <= to_read) {
				while (!read.empty() && read.front() <= heads[head_pointer] + max_time) {
					read.pop_front();
				}
			} else {
				ll d_furthest_right = max((max_time - (heads[head_pointer] - to_read)) / 2, max_time - 2 * (heads[head_pointer] - to_read));
				while(!read.empty() && read.front() <= heads[head_pointer] + d_furthest_right) {
					read.pop_front();
				}
			}
			head_pointer++;
		}
		return true;
	};
	ll l = 0, r = 1e11;
	while(l < r) {
		ll mid = (l + r) / 2;
		read = deque<ll>(_read.begin(), _read.end());
		if (ok(mid)) {
			r = mid;
		} else {
			l = mid + 1;
		}
	}
	cout << l << end1;
}