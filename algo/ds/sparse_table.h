#pragma once
#include "algo/common.h"
#include "algo/ds/common.h"
#include "algo/utils/bits.h"

namespace algo::ds {

template <typename T>
struct sparse_table {
    // Must be constructed with idempotent function. Call init() after if using
    // this constructor.
    sparse_table(index_t _n, const std::function<T(T, T)> &op = min<T>)
        : n(_n), k(utils::lg2(n)), op(op),
          st(std::max<index_t>(k + 1, 1), std::vector<T>(n)) {
    }
    // Must be constructed with idempotent function
    sparse_table(const std::vector<T> &a,
                 const std::function<T(T, T)> &op = min<T>)
        : sparse_table((index_t)a.size(), op) {
        init(a);
    }
    void init(const std::vector<T> &a) {
        assert((index_t)a.size() <= n);
        std::copy(a.begin(), a.end(), st[0].begin());
        for (index_t i = 1; i <= k; i++) {
            for (index_t j = 0; j + (index_t(1) << i) <= n; j++) {
                st[i][j] =
                    op(st[i - 1][j], st[i - 1][j + (index_t(1) << (i - 1))]);
            }
        }
    }
    // Queries on [l, r]
    T query(index_t l, index_t r) {
        index_t i = utils::lg2(r - l + 1);
        return op(st[i][l], st[i][r - (index_t(1) << i) + 1]);
    }
    friend std::ostream &operator<<(std::ostream &os, const sparse_table &t) {
        return os << t.st[0];
    }

private:
    // k is the max level index and is -1 when n is 0, so the row count is
    // floored at 1 to keep level 0 present for init() to copy into.
    index_t n, k;
    std::function<T(T, T)> op;
    std::vector<std::vector<T>> st;
};

} // namespace algo::ds
