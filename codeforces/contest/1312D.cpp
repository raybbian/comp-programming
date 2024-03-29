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

template<typename T>
constexpr T power(T a, ll b) {
	T res = 1;
	for(; b; b /= 2, a *= a) {
		if (b % 2) res *= a;
	}
	return res;
}

template<int P>
struct MInt {
	int x;
	constexpr MInt() : x{} {}
	constexpr MInt(ll x) : x{norm(x % P)} {}

	constexpr int norm(int x) const {
		if(x < 0) {
		    x += P;
		}
		if(x >= P) {
		    x -= P;
		}
		return x;
	}
	constexpr int val() const {
		return x;
	}
	explicit constexpr operator int() const {
		return x;
	}
	constexpr MInt operator-() const {
		MInt res;
		res.x = norm(P - x);
		return res;
	}
	constexpr MInt inv() const {
		assert(x != 0);
		return power(*this, P - 2);
	}
	constexpr MInt &operator*=(MInt rhs) {
		x = 1LL * x * rhs.x % P;
		return *this;
	}
	constexpr MInt &operator+=(MInt rhs) {
		x = norm(x + rhs.x);
		return *this;
	}
	constexpr MInt &operator-=(MInt rhs) {
		x = norm(x - rhs.x);
		return *this;
	}
	constexpr MInt &operator/=(MInt rhs) {
		return *this *= rhs.inv();
	}
	friend constexpr MInt operator*(MInt lhs, MInt rhs) {
		MInt res = lhs;
		res *= rhs;
		return res;
	}
	friend constexpr MInt operator+(MInt lhs, MInt rhs) {
		MInt res = lhs;
		res += rhs;
		return res;
	}
	friend constexpr MInt operator-(MInt lhs, MInt rhs) {
		MInt res = lhs;
		res -= rhs;
		return res;
	}
	friend constexpr MInt operator/(MInt lhs, MInt rhs) {
		MInt res = lhs;
		res /= rhs;
		return res;
	}
	friend constexpr std::istream &operator>>(std::istream &is, MInt &a) {
		ll v;
		is >> v;
		a = MInt(v);
		return is;
	}
	friend constexpr std::ostream &operator<<(std::ostream &os, const MInt &a) {
		return os << a.val();
	}
	friend constexpr bool operator==(MInt lhs, MInt rhs) {
		return lhs.val() == rhs.val();
	}
	friend constexpr bool operator!=(MInt lhs, MInt rhs) {
		return lhs.val() != rhs.val();
	}
};

using modint107 = MInt<1000000007>;
using modint998 = MInt<998244353>;

using mint = modint998;

vector<mint> fact(2e5 + 1);
vector<mint> pow2(2e5 + 1);

void solve() {
	int n, m;
	cin >> n >> m;
	if (n == 2) {
		cout << 0 << ln;
		return;
	}
	cout << fact[m] / (fact[n - 1] * fact[m - n + 1]) * pow2[n - 3] * (n - 2) << ln;
}

int main() {
	cin.tie(nullptr)->sync_with_stdio(false);
	fact[0] = 1;
	pow2[0] = 1;
	for(int i = 1; i <= 2e5+1; i++) {
		fact[i] = i * fact[i - 1];
		pow2[i] = 2 * pow2[i - 1];
	}
	solve();
}