#ifdef LOCAL
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.hpp"
#define dbg(...) cerr << "(" << (#__VA_ARGS__) << "): ", debug(__VA_ARGS__)
#else
#include <bits/stdc++.h>
using namespace std;
#define dbg(...) 42
#endif

//policy based ds
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>
	ordered_set;
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update>
	ordered_multiset; //lower_bound and upper_bound are switched

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
	int n;
	cin >> n;
	vector<int> arr(n + 1);
	int pos = 0, neg = 0;
	int bigPosIn = -1, bigNegIn = -1, bigPos = 0, bigNeg = INT_MAX;
	for(int i = 1; i <= n; i++) {
		cin >> arr[i];
		if (arr[i] > 0) {
			pos++;
			if (arr[i] > bigPos) {
				bigPos = arr[i];
				bigPosIn = i;
			}
		}
		else if (arr[i] < 0) {
			neg++;
			if (arr[i] < bigNeg) {
				bigNeg = arr[i];
				bigNegIn = i;
			}
		}
	}
	//positive only -> add previous number to next number (if dec)
	//make all positive first?
	//negative only -> add next number to prev number (if dec)
	if (pos == 0 && neg == 0) {
		cout << "0\n";
		return;
	}
	//if there are 12 positive and 8 negative, but big neg > big pos, then OK
	//if there are 13 positive and 7 negative, then take 5 to make big pos, and 7 to transform rest OK
	//if there are 8 positive and 12 negative, but big pos > big neg, then OK
	//1 1 1 1 1 1 1 1 1 1 1 1 1 -20 -20 -20 -20 -20 -20 -20
	int amt = 0;
	vector<pii> out;
	auto makeNegPos = [&]() -> void {
		for(int i = 1; i <= n; i++) { //10 max
			if (arr[i] < 0) {
				out.eb(i, bigPosIn);
				amt++;
			}
		}
		for(int i = 1; i < n; i++) { //19 max
			out.eb(i + 1, i);
			amt++;
		}
	};
	auto makePosNeg = [&]() -> void {
		for(int i = 1; i <= n; i++) {
			if (arr[i] > 0) {
				out.eb(i, bigNegIn);
				amt++;
			}
		}
		for(int i = n - 1; i > 0; i--) {
			out.eb(i, i + 1);
			amt++;
		}
	};

	if (bigPos > -1 * bigNeg) {
		if (neg > pos + 4) {
			//make bigNeg bigger
			for(int i = 0; i < 5; i++) {
				out.eb(bigNegIn, bigNegIn);
				amt++;
			}
			makePosNeg();
		} else {
			makeNegPos();
		}
	} else {
		if (pos > neg + 4) {
			//make bigPos bigger
			for(int i = 0; i < 5; i++) {
				out.eb(bigPosIn, bigPosIn);
				amt++;
			}
			makeNegPos();
		} else {
			makePosNeg();
		}
	}
	cout << amt << "\n";
	for(auto &i: out) {
		cout << i.fi << " " << i.se << "\n";
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}