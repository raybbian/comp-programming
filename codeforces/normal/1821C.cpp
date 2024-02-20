#include <bits/stdc++.h>
using namespace std;

#define fi first
#define se second
#define end1 "\n"
#define pb push_back
#define eb emplace_back

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

//debug template
template <typename T>
concept Printable = requires (T &x) {{cout << x} -> same_as<ostream&>;};

template <ranges::range T>
void debug_print(const T &x);

template <Printable T>
void debug_print(const T &x) requires (!is_array<T>::value) {
	cerr << x;
}

template <typename T1, typename T2>
void debug_print(const pair<T1, T2> &x) {
	cerr << "{";
	debug_print(x.fi);
	cerr << " : ";
	debug_print(x.se);
	cerr << "}";
}

template <ranges::range T>
void debug_print(const T &x) {
	cerr << "[";
	bool first = true;
	for(const auto &i : x) {
		if (!first) cerr << ", ";
		first = false;
		debug_print(i);
	}
	cerr << "]";
}

template <typename T>
void debug(const T &x, const string &name = "") {
	if (name.length()) cerr << name << " = "; 
	debug_print(x);
	cerr << end1;
}

//easy modular arithmetic
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
	//for each segment between adjacent letters
	//how many ops it takes to remove:
	//1: 1, 2: 2, 3: 2, 4: 3, 5: 3, 6: 3, 7: 3, 8: 4
	vector<int> pos[26];
	for(int i = 0; i < 26; i++) pos[i].pb(-1);
	for(int i = 0; i < s.length(); i++) {
		pos[s[i] - 'a'].pb(i);
	}
	for(int i = 0; i < 26; i++) pos[i].pb(s.length());
	int best[26];
	memset(best, -1, sizeof(best));
	for(int i = 0; i < 26; i++) {
		for(int j = 1; j < pos[i].size(); j++) {
			int dist = pos[i][j] - pos[i][j - 1] - 1;
			best[i] = max(best[i], dist);
		}
	}
	int ans = INT_MAX;
	for(int i = 0; i < 26; i++) {
		if (best[i] != -1) {
			ans = min(best[i], ans);
		}
	}
	if (ans == 0) {
		cout << 0 << end1;
		return;
	}
	cout << int(log2(ans)) + 1 << end1;
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}