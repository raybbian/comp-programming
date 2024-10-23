#include <bits/stdc++.h>
using namespace std;

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

constexpr long long c_div(long long a, long long b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr long long f_div(long long a, long long b) { return a / b - ((a ^ b) < 0 && a % b); }

#ifdef LOCAL
#include "/home/rayb/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// #define int long long

// insert snippets below:
template <typename T, typename U> T last_true(T l, T r, U f) {
    l--;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l + 1) / 2;
        f(mid) ? l = mid : r = mid - 1;
    }
    return l;
}
template <typename T, typename U> T first_true(T l, T r, U f) {
    r++;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l) / 2;
        f(mid) ? r = mid : l = mid + 1;
    }
    return l;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    rep(i, 1, n) { cin >> a[i]; }

    int ans = 0;
    auto calc = [&](int bit) {
        vector<int> pref_xor(n + 1);
        vector<int> pref_sum(n + 1);
        vector<pair<int, int>> check = {{0, 0}};

        rep(i, 1, n) {
            pref_xor[i] = a[i] & ~((1 << (bit + 1)) - 1);
            pref_xor[i] ^= pref_xor[i - 1];
            check.emplace_back(pref_xor[i], i);
            pref_sum[i] = pref_sum[i - 1] + !!(a[i] & (1 << bit));
        }
        sort(check.begin(), check.end());
        dbg(check);

        vector<int> v(n + 1);
        rep(l, 0, n) {
            if (v[l])
                continue;
            const auto &[l_xor, l_ind] = check[l];
            vector<int> even_ind;
            vector<int> odd_ind;
            rep(r, l, n) {
                const auto &[r_xor, r_ind] = check[r];
                if (l_xor != r_xor)
                    break;

                if (!odd_ind.empty() && r_ind % 2 == 0) {
                    auto check_sum = [&](int test_ind) -> bool {
                        int ll = odd_ind[test_ind];
                        return pref_sum[r_ind] - pref_sum[ll - 1] == r_ind - ll + 1;
                    };
                    int ll_ind = first_true(0, (int)odd_ind.size() - 1, check_sum);
                    if (ll_ind != odd_ind.size() && odd_ind[ll_ind] < r_ind) {
                        ans = max(ans, r_ind - odd_ind[ll_ind] + 1);
                    }
                }
                if (!even_ind.empty() && r_ind % 2 == 1) {
                    auto check_sum = [&](int test_ind) -> bool {
                        int ll = even_ind[test_ind];
                        return pref_sum[r_ind] - pref_sum[ll - 1] == r_ind - ll + 1;
                    };
                    int ll_ind = first_true(0, (int)even_ind.size() - 1, check_sum);
                    if (ll_ind != even_ind.size() && even_ind[ll_ind] < r_ind) {
                        ans = max(ans, r_ind - even_ind[ll_ind] + 1);
                    }
                }

                if (r_ind % 2) {
                    even_ind.push_back(r_ind + 1);
                } else {
                    odd_ind.push_back(r_ind + 1);
                }
                v[r] = true;
            }
        }
    };
    rep(bit, 25) { calc(bit); }
    cout << ans << '\n';
}

signed main() {
#ifndef LOCAL
    cin.tie(nullptr)->sync_with_stdio(false);
#endif
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
