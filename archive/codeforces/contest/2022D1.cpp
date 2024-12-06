#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using db = long double;

#define REP2(i, n) for (int i = 0; i < (n); i++)
#define REP3(i, a, b) for (int i = (a); i <= (b); i++)
#define PER2(i, n) for (int i = (n) - 1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &i : (a))

template <typename T> vector<T> ndvec(size_t size, T initial_value) {
    return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return vector<decltype(inner)>(head, inner);
}

constexpr ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

#ifdef LOCAL
#include "/home/rayb/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

int ask(int i, int j) {
    cout << "? " << i << ' ' << j << '\n';
    cout.flush();
    int resp;
    cin >> resp;
    assert(resp != -1);
    return resp;
}

void solve() {
    int n;
    cin >> n;

    vector<int> team(n + 1, -1);
    team[1] = 0;
    rep(i, 2, n) {
        int resp = ask(i - 1, i);
        if (resp == 1) {
            team[i] = team[i - 1];
        } else {
            team[i] = !(team[i - 1]);
        }
    }
    int test = ask(n, 1);
    if (test == 1) {
        assert(team[n] != team[1]);
    } else {
        assert(team[n] == team[1]);
    }
	int n_test = ask(1, n);
	int a_test = ask(1, 2);
	int b_test = ask(2, 1);
	if (test != n_test && a_test != b_test) {
		cout << "! " << 1 << '\n';
		cout.flush();
		return;
	}

    int l = 1;
    int r = n;
    while (l < r) {
        int mid = l + (r - l + 1) / 2;
        int resp = ask(1, mid);
        if (resp == -1)
            return;
        if (resp == 1) {
            if (team[mid] != team[1]) {
                r = mid - 1;
            } else {
                l = mid;
            }
        } else {
            if (team[mid] == team[1]) {
                r = mid - 1;
            } else {
                l = mid;
            }
        }
    }
	assert(l != 1);

    // ans in l
    cout << "! " << l << '\n';
	cout.flush();
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
        solve();
}

/*
 * SANITY CHECKLIST:
 * 1. re-read the problem
 * 2. consider edge cases (n=1, overflow)
 * 3. come up with counter cases
 * 4. move on to the next problem
 * 5. re-read the problem
 * 6. re-read the problem upside-down
 * 7. re-read the problem backwords
 * 8. fucking re-read the problem
 */

