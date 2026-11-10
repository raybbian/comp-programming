#pragma once
#include "algo/common.h"
#include "algo/ds/monoid.h"
#include "algo/utils/bits.h"

namespace algo::ds {

// Point set, range query. 0-indexed, inclusive.
//   segtree<sum_monoid<int64_t>> t(a);
//   t.set(p, 5);                        // a[p] = 5
//   t.query(l, r);                      // a[l] + ... + a[r]
//
// Custom monoid (this one is sum_monoid):
//   struct M {
//       using Value = int64_t;
//       static Value op(Value a, Value b) { return a + b; }
//       static Value e() { return 0; }
//   };
template <typename M>
struct segtree : M {
    using Value = typename M::Value;

    // Filled with e().
    segtree(int _n) : segtree(std::vector<Value>(_n, M::e())) {
    }
    segtree(const std::vector<Value> &a)
        : n((int)a.size()), sz((int)std::bit_ceil((uint32_t)n)),
          lg(utils::lg2(sz)), d(2 * sz, M::e()) {
        std::copy(a.begin(), a.end(), d.begin() + sz);
        for (int i = sz - 1; i >= 1; i--)
            pull(i);
    }
    // From raw values, when M builds its own leaves (see indexed).
    template <typename U>
        requires requires(U x, int i) { M::leaf(x, i); }
    segtree(const std::vector<U> &a) : segtree(leaves(a)) {
    }
    Value get(int p) const {
        assert(0 <= p && p < n);
        return d[p + sz];
    }
    void set(int p, Value x) {
        assert(0 <= p && p < n);
        p += sz;
        d[p] = x;
        for (int i = 1; i <= lg; i++)
            pull(p >> i);
    }
    template <typename U>
        requires requires(U x, int i) { M::leaf(x, i); }
    void set(int p, const U &x) {
        set(p, M::leaf(x, p));
    }
    // op over a[l..r]
    Value query(int l, int r) const {
        assert(0 <= l && r < n);
        if (l > r) return M::e();
        l += sz, r += sz + 1;
        // Keeps order for non-commutative op.
        Value ml = M::e(), mr = M::e();
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = M::op(ml, d[l++]);
            if (r & 1) mr = M::op(d[--r], mr);
        }
        return M::op(ml, mr);
    }
    Value all() const {
        return d[1];
    }
    // Largest r with g(query(l, r - 1)) true. Needs g(e()) and g monotone.
    //   t.max_right(l, [&](auto v) { return v >= k; });  // min: first i >= l with a[i] < k, else n
    template <typename G>
    int max_right(int l, G g) const {
        assert(0 <= l && l <= n);
        assert(g(M::e()));
        if (l == n) return n;
        l += sz;
        Value sm = M::e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!g(M::op(sm, d[l]))) {
                while (l < sz) {
                    l = 2 * l;
                    if (g(M::op(sm, d[l]))) {
                        sm = M::op(sm, d[l]);
                        l++;
                    }
                }
                return l - sz;
            }
            sm = M::op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }
    // Smallest l with g(query(l, r)) true.
    //   t.min_left(r, [&](auto v) { return v >= k; }) - 1;  // min: last i <= r with a[i] < k, else -1
    template <typename G>
    int min_left(int r, G g) const {
        assert(0 <= r && r < n);
        assert(g(M::e()));
        r += sz + 1;
        Value sm = M::e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!g(M::op(d[r], sm))) {
                while (r < sz) {
                    r = 2 * r + 1;
                    if (g(M::op(d[r], sm))) {
                        sm = M::op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - sz;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
    friend std::ostream &operator<<(std::ostream &os, const segtree &t) {
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
    // Children of k are 2k, 2k+1; leaf i is sz + i.
    int n, sz, lg;
    std::vector<Value> d;

    template <typename U>
    static std::vector<Value> leaves(const std::vector<U> &a) {
        std::vector<Value> ret(a.size());
        for (int i = 0; i < (int)a.size(); i++)
            ret[i] = M::leaf(a[i], i);
        return ret;
    }
    void pull(int k) {
        d[k] = M::op(d[2 * k], d[2 * k + 1]);
    }
};

} // namespace algo::ds
