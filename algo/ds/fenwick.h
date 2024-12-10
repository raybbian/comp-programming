#pragma once
#include "../common.h"

namespace algo::ds {

template <typename T>
struct fenwick {
    fenwick(int _n) : n(_n), bit(n, 0) {
    }
    fenwick(const std::vector<T> &a) : fenwick(sz(a)) {
        for (int i = 0; i < n; i++) {
            bit[i] += a[i];
            int r = i | (i + 1);
            if (r < n) bit[r] += bit[i];
        }
    }
    // Inclusive on [l, r]
    T sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
    T val(int pos) {
        return sum(pos, pos);
    }
    void add(int idx, int delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] += delta;
    }
    friend std::ostream &operator<<(std::ostream &os, fenwick f) {
        os << "[";
        bool first = true;
        for (int i = 0; i < f.n; i++) {
            if (!first) os << ", ";
            first = false;
            os << f.val(i);
        }
        os << "]";
        return os;
    }

private:
    int n;
    std::vector<T> bit;
    T sum(int r) {
        T ret(0);
        for (; r >= 0; r = (r & (r + 1)) - 1)
            ret += bit[r];
        return ret;
    }
};

} // namespace algo::ds
