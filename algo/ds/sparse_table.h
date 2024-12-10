#pragma once
#include "../common.h"
#include "../utils/bits.h"

namespace algo::ds {

template <typename T>
struct sparse_table {
    // Must be constructed with one of min or max
    sparse_table(
        const std::vector<T> &a,
        const std::function<T(T, T)> &op =
            [](const T &a, const T &b) { return std::min(a, b); })
        : n(sz(a)), k(utils::lg2(n)), op(op), st(k + 1, std::vector<T>(n)) {
        std::copy(all(a), st[0].begin());
        for (int i = 1; i <= k; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                st[i][j] = op(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
    }
    T query(int l, int r) {
        int i = utils::lg2(r - l + 1);
        return op(st[i][l], st[i][r - (1 << i) + 1]);
    }

    friend std::ostream &operator<<(std::ostream &os, const sparse_table &t) {
        return os << t.st[0];
    }

private:
    int k, n;
    std::vector<std::vector<T>> st;
    std::function<T(T, T)> op;
};

} // namespace algo::ds
