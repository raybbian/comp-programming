#pragma once
#include "algo/common.h"
#include "algo/utils/bits.h"

namespace algo::ds {

// The monoid a query folds over. repeat(x, len) is x folded with itself len
// times: what a segment of len equal cells collapses to.
template <typename T>
struct sum_monoid {
    using Value = T;
    static Value op(Value a, Value b) {
        return a + b;
    }
    static Value e() {
        return Value(0);
    }
    static Value repeat(Value x, index_t len) {
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
    static Value repeat(Value x, index_t) {
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
    static Value repeat(Value x, index_t) {
        return x;
    }
};

// Adds f to every cell of a segment. The + is on Value, not op, so a range add
// shifts a min or max by f instead of folding f into it.
template <typename M>
struct add_lazy : M {
    using Value = typename M::Value;
    using Update = Value;
    static Update id() {
        return Update(0);
    }
    static Update composition(Update f, Update g) {
        return f + g;
    }
    static Value mapping(Update f, Value x, index_t len) {
        return x + M::repeat(f, len);
    }
};

// Overwrites every cell of a segment. Any Value may be written, so none is free
// to mean "nothing pending" and Update needs the extra nullopt. A write
// discards what it lands on, so of two writes the later wins.
template <typename M>
struct assign_lazy : M {
    using Value = typename M::Value;
    using Update = std::optional<Value>;
    static Update id() {
        return std::nullopt;
    }
    static Update composition(Update f, Update g) {
        return f ? f : g;
    }
    static Value mapping(Update f, Value x, index_t len) {
        return f ? M::repeat(*f, len) : x;
    }
};

// P supplies the monoid (Value, op, e) that queries fold over, the monoid of
// pending updates (Update, composition, id) where composition(f, g) applies g
// first, and mapping(f, x), which applies an update to a whole fold rather than
// to one cell. mapping may take the number of cells x covers as a third
// argument, for updates that scale with segment length. Inheriting P is free
// and keeps its names reachable.
template <typename P>
struct lazy_segtree : P {
    using Value = typename P::Value;
    using Update = typename P::Update;

    // If the default value for leaf elements is not identical (e.g. in the case
    // that indices are stored) then you must use the alternative constructor
    lazy_segtree(index_t _n) : lazy_segtree(std::vector<Value>(_n, P::e())) {
    }
    lazy_segtree(const std::vector<Value> &a)
        : n((index_t)a.size()), sz((index_t)std::bit_ceil((uint32_t)n)),
          lg(utils::lg2(sz)), d(2 * sz, P::e()), lz(sz, P::id()) {
        std::copy(a.begin(), a.end(), d.begin() + sz);
        for (index_t i = sz - 1; i >= 1; i--)
            pull(i);
    }
    Value get(index_t p) {
        assert(0 <= p && p < n);
        p += sz;
        push_down(p);
        return d[p];
    }
    void set(index_t p, Value x) {
        assert(0 <= p && p < n);
        p += sz;
        push_down(p);
        d[p] = x;
        for (index_t i = 1; i <= lg; i++)
            pull(p >> i);
    }
    // Inclusive on [l, r]
    Value query(index_t l, index_t r) {
        assert(0 <= l && r < n);
        if (l > r) return P::e();
        l += sz, r += sz + 1;
        push_down(l, r);
        // Two accumulators keep the fold in index order, which ops that are
        // not commutative need.
        Value ml = P::e(), mr = P::e();
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = P::op(ml, d[l++]);
            if (r & 1) mr = P::op(d[--r], mr);
        }
        return P::op(ml, mr);
    }
    Value all() {
        return d[1];
    }
    // Inclusive on [l, r]
    void apply(index_t l, index_t r, Update f) {
        assert(0 <= l && r < n);
        if (l > r) return;
        l += sz, r += sz + 1;
        push_down(l, r);
        index_t l0 = l, r0 = r;
        for (index_t len = 1; l < r; l >>= 1, r >>= 1, len <<= 1) {
            if (l & 1) all_apply(l++, f, len);
            if (r & 1) all_apply(--r, f, len);
        }
        for (index_t i = 1; i <= lg; i++) {
            if (((l0 >> i) << i) != l0) pull(l0 >> i);
            if (((r0 >> i) << i) != r0) pull((r0 - 1) >> i);
        }
    }
    friend std::ostream &operator<<(std::ostream &os, lazy_segtree t) {
        // A parent precedes its children in index order, so one increasing
        // sweep pushes every pending update out to the leaves.
        for (index_t k = 1; k < t.sz; k++)
            t.push(k, t.sz >> (utils::lg2(k) + 1));
        os << "[";
        bool first = true;
        for (index_t i = 0; i < t.n; i++) {
            if (!first) os << ", ";
            first = false;
            os << t.d[t.sz + i];
        }
        return os << "]";
    }

private:
    // A 1-indexed heap of 2 * sz nodes, the cells past n padded with e(). d[k]
    // is always current; lz[k] is owed to k's children, never to k itself.
    index_t n, sz, lg;
    std::vector<Value> d;
    std::vector<Update> lz;

    void pull(index_t k) {
        d[k] = P::op(d[2 * k], d[2 * k + 1]);
    }
    // len is the number of cells node k covers. Policies whose updates ignore
    // segment length may leave the third parameter of mapping off.
    void all_apply(index_t k, const Update &f, index_t len) {
        if constexpr (requires { P::mapping(f, d[k], len); }) {
            d[k] = P::mapping(f, d[k], len);
        } else {
            d[k] = P::mapping(f, d[k]);
        }
        if (k < sz) lz[k] = P::composition(f, lz[k]);
    }
    // len is the number of cells each child of node k covers
    void push(index_t k, index_t len) {
        all_apply(2 * k, lz[k], len);
        all_apply(2 * k + 1, lz[k], len);
        lz[k] = P::id();
    }
    // Clears every pending update above leaf p
    void push_down(index_t p) {
        for (index_t i = lg; i >= 1; i--)
            push(p >> i, index_t(1) << (i - 1));
    }
    // Same for the two boundary paths of [l, r). A node wholly inside the range
    // is used as a whole and its own value is already current, so nothing below
    // it needs clearing.
    void push_down(index_t l, index_t r) {
        for (index_t i = lg; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i, index_t(1) << (i - 1));
            if (((r >> i) << i) != r) push((r - 1) >> i, index_t(1) << (i - 1));
        }
    }
};

template <typename T>
using add_sum = add_lazy<sum_monoid<T>>;
template <typename T>
using add_min = add_lazy<min_monoid<T>>;
template <typename T>
using add_max = add_lazy<max_monoid<T>>;
template <typename T>
using assign_sum = assign_lazy<sum_monoid<T>>;
template <typename T>
using assign_min = assign_lazy<min_monoid<T>>;
template <typename T>
using assign_max = assign_lazy<max_monoid<T>>;

} // namespace algo::ds
