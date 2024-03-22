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
	int n;
	cin >> n;
	int arr[n];
	map<int, vector<int>> freq;
	unordered_map<int, int> nums;
	for(int i = 0; i < n; i++) {
		cin >> arr[i];
		freq[arr[i]].push_back(i);
		nums[arr[i]]++;
	}
	int mex = -1, previ = -1;
	for(auto &i: freq) {
		if (i.fi != previ + 1) {
			mex = previ + 1;
			break;
		}
		previ = i.fi;
	}
	if (mex == -1) mex = prev(freq.end())->fi + 1;
	//there must be at least one occurence for all numbers less than mex
	//assign all values of mex + 1 to be mex
	//if mex + 1 exists;
	if (nums.count(mex + 1)) {
		for(int i = freq[mex + 1].front(); i <= freq[mex + 1].back(); i++) {
			nums[arr[i]]--;
			if (arr[i] < mex && nums[arr[i]] == 0) {
				cout << "NO" << end1;
				return;
			}
		}
		cout << "YES" << end1;
	} else {
		//if there is nums > mex or nums < mex with freq > num
		if (freq.upper_bound(mex) != freq.end()) {
			cout << "YES" << end1;
			return;
		} else {
			for(auto it = freq.begin(); it != freq.upper_bound(mex); it++) {
				if (it->se.size() > 1) {
					cout << "YES" << end1;
					return;
				}
			}
		}
		cout << "NO" << end1;
	}
}

int main() {
//	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}