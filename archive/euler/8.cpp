#include <bits/stdc++.h>
using namespace std;

// typing is hard
using ll = long long;
using db = long double;
using str = string;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
#define ln "\n"

// loops
#define REP2(i, n) for (int i = 0; i < (n); i++)
#define REP3(i, a, b) for (int i = (a); i <= (b); i++)
#define PER2(i, n) for (int i = (n)-1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &i : (a))

// n-dimensional vector
template <typename T> vector<T> ndvec(size_t size, T initial_value) {
    return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return vector<decltype(inner)>(head, inner);
}

// bitwise ops
constexpr int popcnt(int x) { return __builtin_popcount(x); }            // # of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x))

// better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

// check and assign max and min
template <typename T> bool chk_min(T &a, const T &b) { return b < a ? a = b, 1 : 0; }
template <typename T> bool chk_max(T &a, const T &b) { return a < b ? a = b, 1 : 0; }

#ifdef LOCAL
#include "/home/rayb/Documents/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void solve() {
    string s = "73167176531330624919225119674426574742355349194934"
               "96983520312774506326239578318016984801869478851843"
               "85861560789112949495459501737958331952853208805511"
               "12540698747158523863050715693290963295227443043557"
               "66896648950445244523161731856403098711121722383113"
               "62229893423380308135336276614282806444486645238749"
               "30358907296290491560440772390713810515859307960866"
               "70172427121883998797908792274921901699720888093776"
               "65727333001053367881220235421809751254540594752243"
               "52584907711670556013604839586446706324415722155397"
               "53697817977846174064955149290862569321978468622482"
               "83972241375657056057490261407972968652414535100474"
               "82166370484403199890008895243450658541227588666881"
               "16427171479924442928230863465674813919123162824586"
               "17866458359124566529476545682848912883142607690042"
               "24219022671055626321111109370544217506941658960408"
               "07198403850962455444362981230987879927244284909188"
               "84580156166097919133875499200524063689912560717606"
               "05886116467109405077541002256983155200055935729725"
               "71636269561882670428252483600823257530420752963450";
    ll ans = 0;
    int n = s.length();
    rep(i, n) {
        if (i + 12 >= n)
            break;
        ll temp = 1;
        rep(j, 13) { temp *= s[i + j] - '0'; }
        ans = max(temp, ans);
    }
    cout << ans << endl;
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
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
