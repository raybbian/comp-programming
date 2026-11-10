#pragma once
#include "algo/common.h"

namespace algo::ds {

// op combines, e() is empty, repeat(x, len) is x combined with itself len times.
template <typename T>
struct sum_monoid {
    using Value = T;
    static Value op(Value a, Value b) {
        return a + b;
    }
    static Value e() {
        return Value(0);
    }
    static Value repeat(Value x, int64_t len) {
        return x * len;
    }
};

template <typename T>
struct min_monoid {
    using Value = T;
    static Value op(Value a, Value b) {
        return std::min(a, b);
    }
    static Value e() {
        return std::numeric_limits<Value>::max();
    }
    static Value repeat(Value x, int64_t) {
        return x;
    }
};

template <typename T>
struct max_monoid {
    using Value = T;
    static Value op(Value a, Value b) {
        return std::max(a, b);
    }
    static Value e() {
        return std::numeric_limits<Value>::lowest();
    }
    static Value repeat(Value x, int64_t) {
        return x;
    }
};

// Tags each value of policy P with its index, so queries also say where the
// result came from. For ops that pick one side (min, max) and, in a lazy
// segtree, updates that keep the order of values (add, assign). Leftmost on
// ties, except ties made by an update, which give any of the tied indices.
//   segtree<indexed<min_monoid<int>>> t(a);    // a is a vector<int>
//   lazy_segtree<indexed<add_min<int>>> t(a);
//   t.set(p, x);
//   t.query(l, r);                             // {min, index of it}
template <typename P>
struct indexed : P {
    using Value = std::pair<typename P::Value, int>;
    static Value leaf(const typename P::Value &x, int i) {
        return {x, i};
    }
    static Value op(Value a, Value b) {
        if (a.second == -1) return b;
        if (b.second == -1) return a;
        return P::op(a.first, b.first) == a.first ? a : b;
    }
    static Value e() {
        return {P::e(), -1};
    }
    template <typename F>
    static Value mapping(const F &f, Value x, int64_t len) {
        if (x.second == -1) return x;
        if constexpr (requires { P::mapping(f, x.first, len); }) {
            return {P::mapping(f, x.first, len), x.second};
        } else {
            return {P::mapping(f, x.first), x.second};
        }
    }
};

} // namespace algo::ds
