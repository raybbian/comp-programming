#pragma once
#include "algo/common.h"
#include "algo/ds/monoid.h"

namespace algo::ds {

// segtree over cells lo..hi (inclusive, may be huge or negative) that only
// allocates what it touches: O(log(hi - lo)) nodes per set.
//   sparse_segtree<sum_monoid<int64_t>> t(0, 1e18);  // all e()
//   sparse_segtree<max_monoid<int>> t(-n, n, 0);     // all 0
//   t.set(p, 5);
//   t.query(l, r);                                   // a[l] + ... + a[r]
template <typename M>
struct sparse_segtree : M {
    using Value = typename M::Value;

    sparse_segtree(int64_t _lo, int64_t _hi, Value init = M::e())
        : lo(_lo), hi(_hi), lg(std::bit_width((uint64_t)(hi - lo))),
          dflt(lg + 1, init), nodes(2) {
        assert(lo <= hi);
        for (int h = 1; h <= lg; h++)
            dflt[h] = M::op(dflt[h - 1], dflt[h - 1]);
        nodes[1].v = dflt[lg];
    }
    void reserve(int cnt) {
        nodes.reserve(cnt);
    }
    Value get(int64_t p) const {
        assert(lo <= p && p <= hi);
        int k = 1;
        for (int h = lg; h >= 1 && k; h--)
            k = nodes[k].c[side(p, h)];
        return val(k, 0);
    }
    void set(int64_t p, Value x) {
        assert(lo <= p && p <= hi);
        std::array<int, 64> path;
        int k = 1;
        for (int h = lg; h >= 1; h--) {
            path[h] = k;
            int b = side(p, h);
            if (!nodes[k].c[b]) {
                nodes.emplace_back(); // may reallocate, so index afterwards
                nodes[k].c[b] = (int)nodes.size() - 1;
            }
            k = nodes[k].c[b];
        }
        nodes[k].v = x;
        for (int h = 1; h <= lg; h++) {
            k = path[h];
            nodes[k].v =
                M::op(val(nodes[k].c[0], h - 1), val(nodes[k].c[1], h - 1));
        }
    }
    // op over a[l..r]
    Value query(int64_t l, int64_t r) const {
        assert(lo <= l && r <= hi);
        if (l > r) return M::e();
        return query(1, lg, lo, l, r);
    }
    Value all() const {
        return query(lo, hi);
    }
    // Largest r with g(query(l, r - 1)) true. Needs g(e()) and g monotone.
    //   t.max_right(lo, [&](auto v) { return v < k; });  // sum: k-th one
    template <typename G>
    int64_t max_right(int64_t l, G g) const {
        assert(lo <= l && l <= hi + 1);
        assert(g(M::e()));
        Value sm = M::e();
        return std::min(max_right(1, lg, lo, l, g, sm), hi + 1);
    }
    // Smallest l with g(query(l, r)) true.
    template <typename G>
    int64_t min_left(int64_t r, G g) const {
        assert(lo <= r && r <= hi);
        assert(g(M::e()));
        Value sm = M::e();
        return min_left(1, lg, lo, r, g, sm) + 1;
    }
    // Cells that were set.
    friend std::ostream &operator<<(std::ostream &os, const sparse_segtree &t) {
        bool first = true;
        auto dfs = [&](auto &self, int k, int h, int64_t nl) -> void {
            if (!k) return;
            if (h == 0) {
                if (!first) os << ", ";
                first = false;
                os << nl << ": " << t.nodes[k].v;
                return;
            }
            self(self, t.nodes[k].c[0], h - 1, nl);
            self(self, t.nodes[k].c[1], h - 1, nl + (1LL << (h - 1)));
        };
        os << "{";
        dfs(dfs, 1, t.lg, t.lo);
        return os << "}";
    }

private:
    // Node at height h covers 2^h cells; the root, node 1, is at height lg and
    // starts at lo. Child 0 means absent: all init, worth dflt[h]. Node 0 is
    // that absent node, so its children are absent too.
    struct node {
        Value v;
        int c[2] = {0, 0};
    };
    int64_t lo, hi;
    int lg;
    std::vector<Value> dflt;
    std::vector<node> nodes;

    int side(int64_t p, int h) const {
        return (int)(((p - lo) >> (h - 1)) & 1);
    }
    const Value &val(int k, int h) const {
        return k ? nodes[k].v : dflt[h];
    }
    Value query(int k, int h, int64_t nl, int64_t l, int64_t r) const {
        int64_t nr = nl + (1LL << h) - 1;
        if (r < nl || nr < l) return M::e();
        if (l <= nl && nr <= r) return val(k, h);
        return M::op(query(nodes[k].c[0], h - 1, nl, l, r),
                     query(nodes[k].c[1], h - 1, nl + (1LL << (h - 1)), l, r));
    }
    // First cell >= l where g fails, or the end of the node if none. sm is op
    // over the cells passed so far.
    template <typename G>
    int64_t max_right(int k, int h, int64_t nl, int64_t l, G &g,
                      Value &sm) const {
        int64_t end = nl + (1LL << h);
        if (end <= l) return end;
        if (l <= nl) {
            Value v = M::op(sm, val(k, h));
            if (g(v)) {
                sm = v;
                return end;
            }
            if (h == 0) return nl;
        }
        int64_t mid = nl + (1LL << (h - 1));
        int64_t res = max_right(nodes[k].c[0], h - 1, nl, l, g, sm);
        if (res != mid) return res;
        return max_right(nodes[k].c[1], h - 1, mid, l, g, sm);
    }
    // Last cell <= r where g fails, or one before the node if none.
    template <typename G>
    int64_t min_left(int k, int h, int64_t nl, int64_t r, G &g,
                     Value &sm) const {
        if (r < nl) return nl - 1;
        if (nl + (1LL << h) - 1 <= r) {
            Value v = M::op(val(k, h), sm);
            if (g(v)) {
                sm = v;
                return nl - 1;
            }
            if (h == 0) return nl;
        }
        int64_t mid = nl + (1LL << (h - 1));
        int64_t res = min_left(nodes[k].c[1], h - 1, mid, r, g, sm);
        if (res != mid - 1) return res;
        return min_left(nodes[k].c[0], h - 1, nl, r, g, sm);
    }
};

} // namespace algo::ds
