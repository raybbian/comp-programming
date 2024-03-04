#include <bits/stdc++.h>
using namespace std;

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

//policy based ds
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>; //lower_bound and upper_bound reversed!

//n-dimensional vector
template <typename T>
vector<T> ndvec(size_t size, T initial_value) {
	return vector<T>(size, initial_value);
}
template <typename T, typename... U>
auto ndvec(size_t head, U&&... u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//bitwise ops
constexpr int popcnt(int x) { return __builtin_popcount(x); } //# of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } //floor(log2(x))

//better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

//check and assign max and min
template <typename T>
bool chk_min(T& a, const T& b) { return b < a ? a = b, 1 : 0; }
template <typename T>
bool chk_max(T&a, const T& b) { return a < b ? a = b, 1 : 0; }

//closest element in a set
template <typename T>
auto closest_element(T& set, const typename T::value_type& value) -> decltype(set.begin()) {
	const auto it = set.lower_bound(value);
	if (it == set.begin()) return it;
	const auto prev_it = prev(it);
	return (it == set.end() || value - *prev_it <= *it - value) ? prev_it : it;
}

//binary search
template <typename T, typename U>
T last_true(T l, T r, U f) {
	l--;
	assert(l <= r);
	while(l < r) {
		int mid = l + (r - l + 1) / 2;
		f(mid) ? l = mid : r = mid - 1;
	}
	return l;
}
template <typename T, typename U>
T first_true(T l, T r, U f) {
	r++;
	assert(l <= r);
	while(l < r) {
		int mid = l + (r - l) / 2;
		f(mid) ? r = mid : l = mid + 1;
	}
	return l;
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
	template <typename T, typename U>
	size_t operator()(const pair<T, U>& x) const {
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
#include "/home/rayb/Documents/comp-programming/templates/debug.hpp"
#define dbg(...) debug_first(#__VA_ARGS__, sizeof(#__VA_ARGS__), __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

//insert snippets below:


void solve() {

}

int main() {
	cin.tie(nullptr)->sync_with_stdio(false);
	//for each robot, move forward k seconds
	int n, m, x, k;
	cin >> n >> m >> x >> k;
	auto board = ndvec<char>(n, m, ' ');
	for(int r = 0; r < n; r++) {
		string row;
		cin >> row;
		for(int c = 0; c < m; c++) {
			board[r][c] = row[c];
		}
	}
	vector<pair<int, int>> pos(x);
	vector<char> dir(x);
	auto move = [&](int r, int c, int sec, int r_dir, int c_dir, auto&& move) -> void {
		if (sec == k) {
			dbg("finished", r, c, sec, r_dir, c_dir);
			char _dir = ' ';
			if (r_dir == 1) {
				_dir = 'D';
			} else if (r_dir == -1) {
				_dir = 'U';
			} else if (c_dir == -1) {
				_dir = 'L';
			} else {
				_dir = 'R';
			}
			cout << r + 1 << " " << c + 1 << " " << _dir << ln;
			return;
		}
		if (r + r_dir < 0 || r + r_dir >= n || c + c_dir < 0 || c + c_dir >= m || board[r + r_dir][c + c_dir] == '#') {
			move(r, c, sec + 1, -r_dir, -c_dir, move);
		} else {
			move(r + r_dir, c + c_dir, sec + 1, r_dir, c_dir, move);
		}
	};
	for(int i = 0; i < x; i++) {
		int r_pos, c_pos;
		char dir;
		cin >> r_pos >> c_pos >> dir;
		r_pos--, c_pos--;
		int r_st, c_st;
		r_st = c_st = 0;
		if (dir == 'L') {
			c_st = -1;
		} else if (dir == 'R') {
			c_st = 1;
		} else if (dir == 'U') {
			r_st = -1;
		} else {
			r_st = 1;
		}
		dbg(i);
		move(r_pos, c_pos, 0, r_st, c_st, move);
	}
}