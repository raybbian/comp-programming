#pragma once
#include "algo/common.h"
#include "algo/ds/common.h"
#include "algo/utils/bits.h"

namespace algo::ds {

template <typename T>
struct sparse_table {
    // Must be constructed with idempotent function. Call init() after if using
    // this constructor.
    sparse_table(int _n, const std::function<T(T, T)> &op = min<T>)
        : n(_n), k(utils::lg2(n)), op(op), st(k + 1, std::vector<T>(n)) {
    }
    // Must be constructed with idempotent function
    sparse_table(const std::vector<T> &a,
                 const std::function<T(T, T)> &op = min<T>)
        : sparse_table(sz(a), op) {
        init(a);
    }
    void init(const std::vector<T> &a) {
        assert(sz(a) <= n);
        std::copy(all(a), st[0].begin());
        for (int i = 1; i <= k; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                st[i][j] = op(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
    }
    // Queries on [l, r]
    T query(int l, int r) {
        int i = utils::lg2(r - l + 1);
        return op(st[i][l], st[i][r - (1 << i) + 1]);
    }
    friend std::ostream &operator<<(std::ostream &os, const sparse_table &t) {
        return os << t.st[0];
    }

private:
    int n, k;
    std::function<T(T, T)> op;
    std::vector<std::vector<T>> st;
};

} // namespace algo::ds
