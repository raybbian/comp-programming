#pragma once
#include "algo/common.h"

namespace algo::ds {

template <bool union_by_size = true, bool path_compression = true>
struct dsu {
    dsu(int n) : e(std::vector<int>(n, -1)) {
    }
    int get(int x) {
        if (e[x] < 0) return x;
        if (path_compression) return e[x] = get(e[x]);
        return get(e[x]);
    }
    bool is_same(int a, int b) {
        return get(a) == get(b);
    }
    int size(int x) {
        return -e[get(x)];
    }
    bool unite(int x, int y) {
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
        for (int i = 0; i < sz(s.e); i++) {
            if (s.get(i) == i) {
                if (!first) os << ", ";
                first = false;
                os << "[" << i;
                for (int j = 0; j < sz(s.e); j++) {
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
    std::vector<int> e;
};

} // namespace algo::ds
