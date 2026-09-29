#pragma once
#include "algo/common.h"
#include "algo/utils/bits.h"

namespace algo::ds {

// std::min/std::max can't be passed as a type, so wrap them.
template <typename T>
struct min_op {
    T operator()(T a, T b) const {
        return std::min(a, b);
    }
};

template <typename T>
struct max_op {
    T operator()(T a, T b) const {
        return std::max(a, b);
    }
};

// O(1) range query for ops where op(x, x) == x: min, max, gcd, ...
// No updates. 0-indexed.
//   sparse_table<int> st(a);               // min
//   sparse_table<int, max_op<int>> st(a);  // max
//   st.query(l, r);                        // op over a[l..r]
template <typename T, typename Op = min_op<T>>
struct sparse_table {
    // Allocates only. Call init(a) before querying.
    sparse_table(int _n, Op op = Op())
        : n(_n), k(utils::lg2(n)), op(op),
          st(std::max(k + 1, 1), std::vector<T>(n)) {
    }
    sparse_table(const std::vector<T> &a, Op op = Op())
        : sparse_table((int)a.size(), op) {
        init(a);
    }
    void init(const std::vector<T> &a) {
        assert((int)a.size() <= n);
        std::copy(a.begin(), a.end(), st[0].begin());
        for (int i = 1; i <= k; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                st[i][j] =
                    op(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
    }
    // op over a[l..r]
    T query(int l, int r) {
        int i = utils::lg2(r - l + 1);
        return op(st[i][l], st[i][r - (1 << i) + 1]);
    }
    friend std::ostream &operator<<(std::ostream &os, const sparse_table &t) {
        return os << t.st[0];
    }

private:
    // k = floor(log2(n)), or -1 when n = 0. st keeps at least one row.
    int n, k;
    Op op;
    std::vector<std::vector<T>> st;
};

} // namespace algo::ds
