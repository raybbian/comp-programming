#include <bits/stdc++.h>
using namespace std;

#define fi first
#define se second
#define end1 "\n"

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

template <int mod>
struct modint {
	int val;
	constexpr modint(const ll val = 0) noexcept
		: val(val >= 0 ? val % mod : (mod - (-val) % mod) % mod) {}
	bool operator<(const modint &other) const {
		return val < other.val;		    
	}
	bool operator==(const modint &other) const {
		return val == other.val;
	}
	bool operator!=(const modint &other) const {
		return val != other.val;
	}
	modint &operator+=(const modint &other) {
		if ((val += other.val) >= mod) val -= mod;
		return *this;
	}
	modint &operator-=(const modint &other) {
		if ((val += mod - other.val) >= mod) val -= mod;
		return *this;
	}
	modint &operator*=(const modint &other) {
		val = (int)(1LL * val * other.val % mod);
		return *this;
	}
	modint &operator/=(const modint &other) {
		*this *= other.inverse();
		return *this;
	}
	modint operator-() const {
		return modint(-val);
	}
	modint operator+(const modint &other) const {
		return modint(*this) += other;
	}
	modint operator-(const modint &other) const {
		return modint(*this) -= other;
	}
	modint operator*(const modint &other) const {
		return modint(*this) *= other;
	}
	modint operator/(const modint &other) const {
		return modint(*this) /= other;
	}
	modint inverse() const {
		int a = val, b = mod, u = 1, v = 0, t;
		while (b > 0) {
			t = a / b;
			swap(a -= t * b, b), swap(u -= t * v, v);
		}
		return modint(u);
	}
	modint pow(ll n) const {
		assert(n >= 0);
		modint ret(1), mul(val);
		while (n > 0) {
			if (n & 1) ret *= mul;
			mul *= mul;
			n >>= 1;
		}
		return ret;
	}
};

using modint107 = modint<1000000007>;
using modint998 = modint<998244353>;

using mint = modint107;

void solve() {
	string s;
	cin >> s;
	//1 1 1 1 1 1
	//0 1 1 1 1 1
	//0 0 1 1 1 1
	//0 0 0 1 1 1 1
	//0 0 0 0 1 1 
	//5 is 3x3, 6 is 3x4, 7 is 4x4;
	ll streak = 0, temp = 0;
	for(int i = 0; i < 2 * s.length(); i++) {
		int index = i % s.length();
		if (s[index] == '0') temp = 0;
		else temp++;
		streak = max(streak, temp);
	}
	if (streak > s.length()) {
		//all ones
		cout << s.length() * s.length() << end1;
		return;
	}
	ll l, r;
	if (streak % 2 == 1) l = 1, r = 1;
	else l = 0, r = 1;
	l += streak / 2;
	r += streak / 2;
	cout << l * r << end1;
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}