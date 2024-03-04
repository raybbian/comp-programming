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

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		mint arr[n];
		for(int i = 0; i < n; i++) {
			int c;
			cin >> c;
			arr[i] = c;
		}
		mint dp[n][n + 1]; //index c_i, with m moves
		memset(dp, 0, sizeof(dp));
		for(int i = 0; i < n; i++) {
			dp[i][1] = 1;
		}
		for(int mov = 2; mov <= n; mov++) {
			for(int i = 1; i < n; i++) {
				for(int j = 0; j < i; j++) {
					//go from j to i
					if ((mov - 1) % k != 0 && arr[i] != arr[j]) continue;
					dp[i][mov] += dp[j][mov - 1];
				}
			}
		}
		mint ans = 1;
		for(int size = n - n % k; size > 0; size -= k) {
			mint sum = 0;
			for(int i = 0; i < n; i++) {
				sum += dp[i][size];
			}
			if (sum != 0) {
				ans = sum;
				break;
			}
		}
		cout << ans.val << end1;
	}
}