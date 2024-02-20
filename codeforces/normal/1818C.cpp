#ifdef LOCAL
#include "/home/rayb/Documents/Coding/templates/debug.h"
#define dbg(...) cerr << "(" << (#__VA_ARGS__) << "): ", debug(__VA_ARGS__)
#else
#include <bits/stdc++.h>
using namespace std;
#define dbg(...) 42
#endif

//typing is hard
using ll = long long;
using db = long double;
using str = string;
 
//ez pairs
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
 
//ez containers
#define end1 "\n"
#define pb push_back
#define eb emplace_back
#define ins insert
#define sz(x) int((x).size())
#define beg(x) begin(x)
#define all(x) beg(x), end(x)
 
//n-dimensional vector
template <class T>
vector<T> ndvec(size_t size, T initialValue) {
	return vector<T>(size, initialValue);
}
template <class T, class... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}
 
//better math
ll cdiv(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll fdiv(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }
 
//closest element in a set
template <class T>
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
	template <class T, class U>
	size_t operator()(const pair<T, U> &x) const {
		uint64_t a = (*this)(x.fi);
		uint64_t b = (*this)(x.se);
		return a + 3 * b;
	}
};
 
//grid problems
vector<pii> stepsOrthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> stepsDiagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
 
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> arr(n);
    vector<int> pref(n);
    for(int i = 0; i < n; i++) cin >> arr[i];
    for(int i = 2; i < n; i++) {
        pref[i] = pref[i - 1];
        if (arr[i - 2] >= arr[i - 1] && arr[i - 1] >= arr[i]) {
            pref[i]++; //amount of chars needed to delete here increases
        }
    }
    dbg(pref);
    for(int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        l--, r--;
        if (r - l + 1 <= 2) cout << r - l + 1 << end1;
        else if (l == 0) cout << r - l + 1 - pref[r] << end1;
        else cout << r - l + 1 - (pref[r] - pref[l + 1]) << end1;
        dbg(l, r);
    }
}
 
int main() {
	cin.tie(0)->sync_with_stdio(false);
    solve();
}