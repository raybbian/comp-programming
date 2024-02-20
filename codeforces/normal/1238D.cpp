#include <bits/stdc++.h>
using namespace std;

//policy based ds
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
typedef tree<pair<int, int>, null_type, less<pair<int, int>>, rb_tree_tag, tree_order_statistics_node_update> ordered_multiset; //use index as pair second

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

//n-dimensional vector
template <typename T>
vector<T> ndvec(size_t size, T initial_value) {
	return vector<T>(size, initial_value);
}
template <typename T, typename... U>
auto ndvec(size_t head, U &&...u) {
	auto inner = ndvec<T>(u...);
	return vector<decltype(inner)>(head, inner);
}

//better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

//closest element in a set
template <typename T>
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
	template <typename T1, typename T2>
	size_t operator()(const pair<T1, T2> &x) const {
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
#include "/home/rayb/OneDrive/Documents/CompGramming/Templates/debug.hpp"
#define dbg(...) debug_first(#__VA_ARGS__, sizeof(#__VA_ARGS__), __VA_ARGS__)
#else
#define dbg(...) 42
#endif

//insert snippets below:


void solve() {

}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	//good concatenate good is also good
	//iterate from 1 to n
	//if 1...i is good, store i > a[]
	//if all next good strings have a_1...i, a_2...i as good substr as well
	//what if we find un-good substrings?
	//substring is un-good if new parsed character != first char
	//maintain freq table of A, B
	//if next is B, then there are count(A) bad substrings
	//if next is A, then there are count(B) bad substrings
	//how many possible bad substring?
	//01
	//001
	//0001
	//0101
	//if there is same letter x position away, then it is good letter
	//is a good substring until that letter is removed
	//01010111
	//100101
	int n;
	string s;
	cin >> n >> s;
	ll ans = (ll)n * (n - 1) / 2;
	auto run = [&](int d) -> void {
		set<int> a, b;
		for(int i = 0; i < n; i++) {
			dbg(a, b);
			if(s[i] == 'A') {
				//get distance to last a
				if(a.begin() == a.end()) {
					//set is empty
					ans -= max(i - d, 0);
				} else {
					auto last = prev(a.end());
					ans -= max(0, i - d - *last - 1);
				}
				a.ins(i);
			} else {
				if(b.begin() == b.end()) {
					ans -= max(i - d, 0);
				} else {
					auto last = prev(b.end());
					ans -= max(0, i - d - *last - 1);
				}
				b.ins(i);
			}
			dbg(ans);
		}
	};
	run(0);
	reverse(s.begin(), s.end());
	run(1);
	cout << ans << ln;
}