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

pll dfs(ll h, ll w, int type, vector<bool> &v, map<ll, vector<pll>> &byH, map<ll, vector<pll>> &byW) {
//	cout << "rect left: " << h << " " << w << end1;
	ll hhh = h, www = w;
	while(h > 0 && w > 0) {
		int rectanglesRemoved = 0;
		if (type == 1) {
			for(auto &i: byH[h]) {
				if (v[i.se]) continue;
				v[i.se] = true;
				rectanglesRemoved++;
				w -= i.fi;
			}
		} else {
			for(auto &i: byW[w]) {
				if (v[i.se]) continue;
				v[i.se] = true;
				rectanglesRemoved++;
				h -= i.fi;
			}
		}
		if (rectanglesRemoved == 0) return {-1, -1};
		type = (type == 1) ? 2 : 1;
	}
	return {hhh, www};
}

void solve() {
	int n;
	cin >> n;
	vector<pll> rect;
	map<ll, vector<pll>> byH, byW;
	ll totalA = 0;
	for(int i = 0; i < n; i++) {
		ll a, b;
		cin >> a >> b;
		rect.push_back({a, b});
		byH[a].push_back({b, i});
		byW[b].push_back({a, i});
		totalA += a * b;
	}
	
	set<pll> ans;

//	cout << "totalA: " << totalA << end1;
	ll maxH = prev(byH.end())->fi;
	vector<bool> v(n, false);
	if (totalA % maxH == 0) {
		ll compW = totalA / maxH;
//		cout << "Consider: " << maxH << " " << compW << end1;
		pll res = dfs(maxH, compW, 1, v, byH, byW);
		if (res.fi >= 0) ans.insert(res);
	}

	ll maxW = prev(byW.end())->fi;
	v = vector<bool>(n, false);
	if(totalA % maxW == 0) {
		ll compH = totalA / maxW;
//		cout << "Consider: " << maxW << " " << compH << end1;
		pll res = dfs(compH, maxW, 2, v, byH, byW);
		if (res.fi >= 0) ans.insert(res);
	}

	cout << ans.size() << end1;
	for(auto &i: ans) {
		cout << i.fi << " " << i.se << end1;
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