#pragma once
#include "algo/common.h"

namespace algo::ds {

template <bool union_by_size = true, bool path_compression = true>
struct dsu {
    dsu(index_t n) : e(std::vector<index_t>(n, -1)) {
    }
    index_t get(index_t x) {
        if (e[x] < 0) return x;
        if (path_compression) return e[x] = get(e[x]);
        return get(e[x]);
    }
    bool is_same(index_t a, index_t b) {
        return get(a) == get(b);
    }
    index_t size(index_t x) {
        return -e[get(x)];
    }
    bool unite(index_t x, index_t y) {
        x = get(x), y = get(y);
        if (x == y) return false;
        if (union_by_size && e[x] > e[y]) std::swap(x, y);
        e[x] += e[y];
        e[y] = x;
        return true;
    }
    friend std::ostream &operator<<(std::ostream &os, dsu s) {
        os << "[";
        bool first = true;
        for (index_t i = 0; i < (index_t)s.e.size(); i++) {
            if (s.get(i) == i) {
                if (!first) os << ", ";
                first = false;
                os << "[" << i;
                for (index_t j = 0; j < (index_t)s.e.size(); j++) {
                    if (j != i && s.get(j) == i) {
                        os << ", " << j;
                    }
                }
                os << "]";
            }
        }
        return os << "]";
    }

private:
    // A root stores the negated size of its component, so this relies on
    // index_t being signed.
    std::vector<index_t> e;
};

} // namespace algo::ds
