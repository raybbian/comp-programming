#pragma once
#include <algo/common.h>

template <typename T> struct RUPQ {
    const T ID{0};
    T cmb(T a, T b) { return a + b; }
    int n;
    vector<T> seg;
    RUPQ(int _n) {
        for (n = 1; n < _n;)
            n *= 2;
        seg.assign(2 * n, ID);
    }
    void upd(int l, int r, T val) { // inclusive
        for (l += n, r += n + 1; l < r; l /= 2, r /= 2) {
            if (l & 1) {
                seg[l] = cmb(seg[l], val);
                l++;
            }
            if (r & 1) {
                r--;
                seg[r] = cmb(seg[r], val);
            }
        }
    }
    T query(int p) {
        T res = ID;
        for (p += n; p; p /= 2)
            res = cmb(res, seg[p]);
        return res;
    }
    void push() { // order of modification cannot affect result
        for (int i = 1; i < n; i++) {
            seg[2 * i] = cmb(seg[2 * i], seg[i]);
            seg[2 * i + 1] = cmb(seg[2 * i + 1], seg[i]);
            seg[i] = ID;
        }
    }
};
