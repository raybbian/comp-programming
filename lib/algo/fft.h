#pragma once
#ifndef PREPROCESS
#include <bits/stdc++.h>
using namespace std;
#endif

// Copied from https://gist.github.com/Chillee/4982ba0840745f63f3771bd84f280557#file-ntt-cpp
template <int maxn> struct FFT {
    constexpr static int lg2(int n) { return 32 - __builtin_clz(n - 1); }
    const static int MAXN = 1 << lg2(maxn);
    using cpx = complex<double>;

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
        for (int i = 0; i < sz; i++)
            res[i] = out[i].imag() / (4 * n);
        return res;
    }
};
