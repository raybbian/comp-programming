#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using db = long double;

#define REP2(i, n) for (int i = 0; i < int(n); i++)
#define REP3(i, a, b) for (int i = (a); i <= int(b); i++)
#define PER2(i, n) for (int i = (n) - 1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= int(b); i--)
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

/**
 * Copied from https://gist.github.com/Chillee/4982ba0840745f63f3771bd84f280557#file-ntt-cpp
 */

template <int maxn> struct FFT {
    constexpr static int lg2(int n) { return 32 - __builtin_clz(n - 1); }
    const static int MAXN = 1 << lg2(maxn);
    typedef complex<double> cpx;
    int rev[MAXN];
    cpx rt[MAXN];
    FFT() {
        rt[1] = cpx{1, 0};
        for (int k = 2; k < MAXN; k *= 2) {
            cpx z[] = {1, polar(1.0, numbers::pi_v<double> / k)};
            for (int i = k; i < 2 * k; i++)
                rt[i] = rt[i / 2] * z[i & 1];
        }
    }
    void fft(cpx *a, int n) {
        for (int i = 0; i < n; i++)
            rev[i] = (rev[i / 2] | (i & 1) << lg2(n)) / 2;
        for (int i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);
        for (int k = 1; k < n; k *= 2)
            for (int i = 0; i < n; i += 2 * k)
                for (int j = 0; j < k; j++) {
                    auto x = (double *)&rt[j + k], y = (double *)&a[i + j + k];
                    cpx z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
                    a[i + j + k] = a[i + j] - z;
                    a[i + j] += z;
                }
    }

    cpx in[MAXN], out[MAXN];
    vector<double> multiply(const vector<double> &a, const vector<double> &b) {
        if (a.empty() || b.empty())
            return {};
        int sz = a.size() + b.size() - 1, n = 1 << lg2(sz);
        vector<double> res(sz);
        fill(in, in + n, 0), fill(out, out + n, 0);
        copy(a.begin(), a.end(), begin(in));
        for (int i = 0; i < (int)b.size(); i++)
            in[i].imag(b[i]);
        fft(in, n);
        for (int i = 0; i < n; i++)
            in[i] *= in[i];
        for (int i = 0; i < n; i++)
            out[i] = in[(n - i) & (n - 1)] - conj(in[i]);
        fft(out, n);
        for (int i = 0; i < sz; i++) {
            double o = out[i].imag() / (4 * n);
            res[i] = min(o, 1.0);
            res[i] = round(res[i]);
        }
        return res;
    }
};

FFT<1'000'005> fft;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];
    int max_a = *max_element(a.begin(), a.end());

    vector<double> poly(max_a + 1);
    rep(i, n) { poly[a[i]] = 1; }

    auto calc = [&](int num, auto &&calc) -> vector<double> {
        dbg(num);
        if (num == 1)
            return poly;
        if (num % 2 == 1) {
            return fft.multiply(poly, calc(num - 1, calc));
        }

        vector<double> res = calc(num / 2, calc);
        return fft.multiply(res, res);
    };
    vector<double> res = calc(k, calc);

    rep(i, (int)res.size()) {
        if (res[i] > 0.1) {
            cout << i << ' ';
        }
    }
    cout << '\n';
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
