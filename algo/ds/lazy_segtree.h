#pragma once
#include "algo/common.h"
#include "algo/utils/bits.h"

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
    static Value repeat(Value x, int len) {
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
    static Value repeat(Value x, int) {
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
    static Value repeat(Value x, int) {
        return x;
    }
};

// Range add.
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
    static Value mapping(Update f, Value x, int len) {
        return x + M::repeat(f, len);
    }
};

// Range assign. nullopt = no update.
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
    static Value mapping(Update f, Value x, int len) {
        return f ? M::repeat(*f, len) : x;
    }
};

// Range update, range query. 0-indexed, inclusive.
//   lazy_segtree<add_sum<int64_t>> t(a);  // aliases at bottom
//   t.apply(l, r, 5);                     // a[l..r] += 5
//   t.query(l, r);                        // a[l] + ... + a[r]
//
// Custom policy (this one is add_sum):
//   struct P {
//       using Value = int64_t;
//       using Update = int64_t;
//       static Value op(Value a, Value b) { return a + b; }
//       static Value e() { return 0; }
//       static Update id() { return 0; }
//       static Update composition(Update f, Update g) { return f + g; }  // g then f
//       static Value mapping(Update f, Value x, int len) { return x + f * len; }  // len optional
//   };
template <typename P>
struct lazy_segtree : P {
    using Value = typename P::Value;
    using Update = typename P::Update;

    // Filled with e().
    lazy_segtree(int _n) : lazy_segtree(std::vector<Value>(_n, P::e())) {
    }
    lazy_segtree(const std::vector<Value> &a)
        : n((int)a.size()), sz((int)std::bit_ceil((uint32_t)n)),
          lg(utils::lg2(sz)), d(2 * sz, P::e()), lz(sz, P::id()) {
        std::copy(a.begin(), a.end(), d.begin() + sz);
        for (int i = sz - 1; i >= 1; i--)
            pull(i);
    }
    Value get(int p) {
        assert(0 <= p && p < n);
        p += sz;
        push_down(p);
        return d[p];
    }
    void set(int p, Value x) {
        assert(0 <= p && p < n);
        p += sz;
        push_down(p);
        d[p] = x;
        for (int i = 1; i <= lg; i++)
            pull(p >> i);
    }
    // op over a[l..r]
    Value query(int l, int r) {
        assert(0 <= l && r < n);
        if (l > r) return P::e();
        l += sz, r += sz + 1;
        push_down(l, r);
        // Keeps order for non-commutative op.
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
    // f on each of a[l..r]
    void apply(int l, int r, Update f) {
        assert(0 <= l && r < n);
        if (l > r) return;
        l += sz, r += sz + 1;
        push_down(l, r);
        int l0 = l, r0 = r;
        for (int len = 1; l < r; l >>= 1, r >>= 1, len <<= 1) {
            if (l & 1) all_apply(l++, f, len);
            if (r & 1) all_apply(--r, f, len);
        }
        for (int i = 1; i <= lg; i++) {
            if (((l0 >> i) << i) != l0) pull(l0 >> i);
            if (((r0 >> i) << i) != r0) pull((r0 - 1) >> i);
        }
    }
    // Largest r with g(query(l, r - 1)) true. Needs g(e()) and g monotone.
    //   t.max_right(l, [&](auto v) { return v >= k; });  // min: first i >= l with a[i] < k, else n
    template <typename G>
    int max_right(int l, G g) {
        assert(0 <= l && l <= n);
        assert(g(P::e()));
        if (l == n) return n;
        l += sz;
        push_down(l);
        Value sm = P::e();
        int len = 1;
        do {
            while (l % 2 == 0) l >>= 1, len <<= 1;
            if (!g(P::op(sm, d[l]))) {
                while (l < sz) {
                    push(l, len >> 1);
                    l = 2 * l, len >>= 1;
                    if (g(P::op(sm, d[l]))) {
                        sm = P::op(sm, d[l]);
                        l++;
                    }
                }
                return l - sz;
            }
            sm = P::op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }
    // Smallest l with g(query(l, r)) true.
    //   t.min_left(r, [&](auto v) { return v >= k; }) - 1;  // min: last i <= r with a[i] < k, else -1
    template <typename G>
    int min_left(int r, G g) {
        assert(0 <= r && r < n);
        assert(g(P::e()));
        r += sz;
        push_down(r);
        r++;
        Value sm = P::e();
        int len = 1;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1, len <<= 1;
            if (!g(P::op(d[r], sm))) {
                while (r < sz) {
                    push(r, len >> 1);
                    r = 2 * r + 1, len >>= 1;
                    if (g(P::op(d[r], sm))) {
                        sm = P::op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - sz;
            }
            sm = P::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
    friend std::ostream &operator<<(std::ostream &os, lazy_segtree t) {
        // Push everything to the leaves.
        for (int k = 1; k < t.sz; k++)
            t.push(k, t.sz >> (utils::lg2(k) + 1));
        os << "[";
        bool first = true;
        for (int i = 0; i < t.n; i++) {
            if (!first) os << ", ";
            first = false;
            os << t.d[t.sz + i];
        }
        return os << "]";
    }

private:
    // Children of k are 2k, 2k+1; leaf i is sz + i. lz[k] is pending for k's children.
    int n, sz, lg;
    std::vector<Value> d;
    std::vector<Update> lz;

    void pull(int k) {
        d[k] = P::op(d[2 * k], d[2 * k + 1]);
    }
    // f on node k, which covers len cells.
    void all_apply(int k, const Update &f, int len) {
        if constexpr (requires { P::mapping(f, d[k], len); }) {
            d[k] = P::mapping(f, d[k], len);
        } else {
            d[k] = P::mapping(f, d[k]);
        }
        if (k < sz) lz[k] = P::composition(f, lz[k]);
    }
    // lz[k] to k's children, each covering len cells.
    void push(int k, int len) {
        all_apply(2 * k, lz[k], len);
        all_apply(2 * k + 1, lz[k], len);
        lz[k] = P::id();
    }
    // Push everything above leaf p.
    void push_down(int p) {
        for (int i = lg; i >= 1; i--)
            push(p >> i, 1 << (i - 1));
    }
    // Push everything above both ends of [l, r).
    void push_down(int l, int r) {
        for (int i = lg; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i, 1 << (i - 1));
            if (((r >> i) << i) != r) push((r - 1) >> i, 1 << (i - 1));
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
