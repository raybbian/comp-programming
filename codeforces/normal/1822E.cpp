#ifdef LOCAL
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.h"
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

//char to int
int ctoi(char c) { return int(c) - 'a'; }

//grid problems
vector<pii> stepsOrthogonal = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
vector<pii> stepsDiagonal = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
vector<pii> steps8dirs = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

void solve() {
	int n;
	cin >> n;
	string s;
	cin >> s;
	if(n % 2 == 1) {
		cout << "-1\n";
		return;
	}
	vector<char> word(n + 1);
	vector<int> freq(26);
	for(int i = 0; i < n; i++) {
		word[i + 1] = s[i];
		freq[ctoi(s[i])]++;
		if (freq[ctoi(s[i])] > n / 2) {
			cout << "-1\n";
			return;
		}
	}
	dbg(word);
	vector<int> needMoveFreq(26);
	for(int i = 1; i <= n / 2; i++) {
		if(word[i] == word[n - i + 1]) {
			needMoveFreq[ctoi(word[i])]++;
		}
	}
	multiset<int> order;
	for(int i = 0; i < 26; i++) {
		if(needMoveFreq[i] == 0) continue;
		order.insert(needMoveFreq[i]);
	}
	int ans = 0;
	dbg(order);
	while(order.size() > 1) {
		int back = *prev(order.end());
		int back2 = *prev(prev(order.end()));
		order.erase(prev(order.end()));
		order.erase(prev(order.end()));
		if (back != 1) {
			order.insert(back - 1);
		}
		if (back2 != 1) {
			order.insert(back2 - 1);
		}
		ans++;
		dbg(order);

	}
	if(order.size()) {
		ans += *order.begin();
	}
	cout << ans << "\n";

}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	for(int i = 0; i < t; i++) {
		solve();
	}
}