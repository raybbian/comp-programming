#pragma once
#include "algo/common.h"

namespace algo::ds {

template <typename T>
struct fenwick {
    fenwick(index_t _n) : n(_n), bit(n, 0) {
    }
    fenwick(const std::vector<T> &a) : fenwick((index_t)a.size()) {
        for (index_t i = 0; i < n; i++) {
            bit[i] += a[i];
            index_t r = i | (i + 1);
            if (r < n) bit[r] += bit[i];
        }
    }
    // Inclusive on [l, r]
    T sum(index_t l, index_t r) {
        return sum(r) - sum(l - 1);
    }
    T val(index_t pos) {
        return sum(pos, pos);
    }
    void add(index_t idx, T delta) {
        for (; idx < n; idx = idx | (idx + 1))
            bit[idx] += delta;
    }
    friend std::ostream &operator<<(std::ostream &os, fenwick f) {
        os << "[";
        bool first = true;
        for (index_t i = 0; i < f.n; i++) {
            if (!first) os << ", ";
            first = false;
            os << f.val(i);
        }
        os << "]";
        return os;
    }

private:
    index_t n;
    std::vector<T> bit;
    T sum(index_t r) {
        T ret(0);
        for (; r >= 0; r = (r & (r + 1)) - 1)
            ret += bit[r];
        return ret;
    }
};

} // namespace algo::ds
