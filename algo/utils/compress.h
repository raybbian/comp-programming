#pragma once
#include "algo/common.h"

namespace algo::utils {

// Coordinate compression: maps the distinct values of a onto [0, size()) in
// sorted order.
//   compressor<int64_t> c(a);
//   c.idx(x);       // index of x; if absent, number of values < x
//   c.val(i);       // value at index i
//   c.has(x);
//   c.compress(a);  // idx of every element
template <typename T>
struct compressor {
    compressor(std::vector<T> a) : vals(std::move(a)) {
        std::sort(vals.begin(), vals.end());
        vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    }
    int size() const {
        return (int)vals.size();
    }
    int idx(const T &x) const {
        return (int)(std::lower_bound(vals.begin(), vals.end(), x) -
                     vals.begin());
    }
    const T &val(int i) const {
        return vals[i];
    }
    bool has(const T &x) const {
        int i = idx(x);
        return i < size() && vals[i] == x;
    }
    std::vector<int> compress(const std::vector<T> &a) const {
        std::vector<int> ret(a.size());
        for (int i = 0; i < (int)a.size(); i++)
            ret[i] = idx(a[i]);
        return ret;
    }
    friend std::ostream &operator<<(std::ostream &os, const compressor &c) {
        return os << c.vals;
    }

private:
    std::vector<T> vals;
};

} // namespace algo::utils
