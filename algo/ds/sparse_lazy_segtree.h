#pragma once
#include "algo/common.h"
#include "algo/ds/lazy_segtree.h"

namespace algo::ds {

// lazy_segtree over cells lo..hi (inclusive, may be huge or negative) that
// only allocates what it touches: O(log(hi - lo)) nodes per operation, queries
// included. Takes the same policies.
//   sparse_lazy_segtree<add_sum<int64_t>> t(0, 1e18);  // all e()
//   sparse_lazy_segtree<add_min<int>> t(-n, n, 0);     // all 0
//   t.apply(l, r, 5);                                  // a[l..r] += 5
//   t.query(l, r);                                     // a[l] + ... + a[r]
template <typename P>
struct sparse_lazy_segtree : P {
    using Value = typename P::Value;
    using Update = typename P::Update;

    sparse_lazy_segtree(int64_t _lo, int64_t _hi, Value init = P::e())
        : lo(_lo), hi(_hi), lg(std::bit_width((uint64_t)(hi - lo))),
          dflt(lg + 1, init), nodes(1) {
        assert(lo <= hi);
        for (int h = 1; h <= lg; h++)
            dflt[h] = P::op(dflt[h - 1], dflt[h - 1]);
        nodes.emplace_back(dflt[lg]);
    }
    void reserve(int cnt) {
        nodes.reserve(cnt);
    }
    Value get(int64_t p) {
        assert(lo <= p && p <= hi);
        int k = 1;
        for (int h = lg; h >= 1; h--) {
            push(k, h);
            k = nodes[k].c[side(p, h)];
        }
        return nodes[k].v;
    }
    void set(int64_t p, Value x) {
        assert(lo <= p && p <= hi);
        std::array<int, 64> path;
        int k = 1;
        for (int h = lg; h >= 1; h--) {
            path[h] = k;
            push(k, h);
            k = nodes[k].c[side(p, h)];
        }
        nodes[k].v = x;
        for (int h = 1; h <= lg; h++)
            pull(path[h]);
    }
    // op over a[l..r]
    Value query(int64_t l, int64_t r) {
        assert(lo <= l && r <= hi);
        if (l > r) return P::e();
        return query(1, lg, lo, l, r);
    }
    Value all() {
        return query(lo, hi);
    }
    // f on each of a[l..r]
    void apply(int64_t l, int64_t r, Update f) {
        assert(lo <= l && r <= hi);
        if (l > r) return;
        apply(1, lg, lo, l, r, f);
    }
    // Largest r with g(query(l, r - 1)) true. Needs g(e()) and g monotone.
    //   t.max_right(l, [&](auto v) { return v >= k; });  // min: first i >= l
    //                                                    // with a[i] < k
    template <typename G>
    int64_t max_right(int64_t l, G g) {
        assert(lo <= l && l <= hi + 1);
        assert(g(P::e()));
        Value sm = P::e();
        return std::min(max_right(1, lg, lo, l, g, sm), hi + 1);
    }
    // Smallest l with g(query(l, r)) true.
    template <typename G>
    int64_t min_left(int64_t r, G g) {
        assert(lo <= r && r <= hi);
        assert(g(P::e()));
        Value sm = P::e();
        return min_left(1, lg, lo, r, g, sm) + 1;
    }
    // Allocated blocks, as cell or [l, r] to the op over it.
    friend std::ostream &operator<<(std::ostream &os, sparse_lazy_segtree t) {
        bool first = true;
        auto dfs = [&](auto &self, int k, int h, int64_t nl) -> void {
            if (nl > t.hi) return;
            if (!t.nodes[k].c[0]) {
                if (!first) os << ", ";
                first = false;
                int64_t nr = std::min<int64_t>(nl + (1LL << h) - 1, t.hi);
                if (nl == nr) {
                    os << nl;
                } else {
                    os << "[" << nl << ", " << nr << "]";
                }
                os << ": " << t.query(nl, nr);
                return;
            }
            t.push(k, h);
            self(self, t.nodes[k].c[0], h - 1, nl);
            self(self, t.nodes[k].c[1], h - 1, nl + (1LL << (h - 1)));
        };
        os << "{";
        dfs(dfs, 1, t.lg, t.lo);
        return os << "}";
    }

private:
    // Node at height h covers 2^h cells; the root, node 1, is at height lg and
    // starts at lo. Children are made in pairs on the first push, a fresh one
    // being all init, worth dflt[h]. lz is pending for the children.
    struct node {
        Value v;
        Update lz = P::id();
        int c[2] = {0, 0};
        node(Value _v = P::e()) : v(_v) {
        }
    };
    int64_t lo, hi;
    int lg;
    std::vector<Value> dflt;
    std::vector<node> nodes;

    int side(int64_t p, int h) const {
        return (int)(((p - lo) >> (h - 1)) & 1);
    }
    void pull(int k) {
        nodes[k].v = P::op(nodes[nodes[k].c[0]].v, nodes[nodes[k].c[1]].v);
    }
    // f on node k at height h.
    void all_apply(int k, int h, const Update &f) {
        Value &v = nodes[k].v;
        int64_t len = 1LL << h;
        if constexpr (requires { P::mapping(f, v, len); }) {
            v = P::mapping(f, v, len);
        } else {
            v = P::mapping(f, v);
        }
        if (h) nodes[k].lz = P::composition(f, nodes[k].lz);
    }
    void push(int k, int h) {
        if (!nodes[k].c[0]) {
            for (int b = 0; b < 2; b++) {
                // may reallocate, so index afterwards
                nodes.emplace_back(dflt[h - 1]);
                nodes[k].c[b] = (int)nodes.size() - 1;
            }
        }
        all_apply(nodes[k].c[0], h - 1, nodes[k].lz);
        all_apply(nodes[k].c[1], h - 1, nodes[k].lz);
        nodes[k].lz = P::id();
    }
    Value query(int k, int h, int64_t nl, int64_t l, int64_t r) {
        int64_t nr = nl + (1LL << h) - 1;
        if (r < nl || nr < l) return P::e();
        if (l <= nl && nr <= r) return nodes[k].v;
        push(k, h);
        Value a = query(nodes[k].c[0], h - 1, nl, l, r);
        return P::op(a,
                     query(nodes[k].c[1], h - 1, nl + (1LL << (h - 1)), l, r));
    }
    void apply(int k, int h, int64_t nl, int64_t l, int64_t r,
               const Update &f) {
        int64_t nr = nl + (1LL << h) - 1;
        if (r < nl || nr < l) return;
        if (l <= nl && nr <= r) return all_apply(k, h, f);
        push(k, h);
        apply(nodes[k].c[0], h - 1, nl, l, r, f);
        apply(nodes[k].c[1], h - 1, nl + (1LL << (h - 1)), l, r, f);
        pull(k);
    }
    // First cell >= l where g fails, or the end of the node if none. sm is op
    // over the cells passed so far.
    template <typename G>
    int64_t max_right(int k, int h, int64_t nl, int64_t l, G &g, Value &sm) {
        int64_t end = nl + (1LL << h);
        if (end <= l) return end;
        if (l <= nl) {
            Value v = P::op(sm, nodes[k].v);
            if (g(v)) {
                sm = v;
                return end;
            }
            if (h == 0) return nl;
        }
        push(k, h);
        int64_t mid = nl + (1LL << (h - 1));
        int64_t res = max_right(nodes[k].c[0], h - 1, nl, l, g, sm);
        if (res != mid) return res;
        return max_right(nodes[k].c[1], h - 1, mid, l, g, sm);
    }
    // Last cell <= r where g fails, or one before the node if none.
    template <typename G>
    int64_t min_left(int k, int h, int64_t nl, int64_t r, G &g, Value &sm) {
        if (r < nl) return nl - 1;
        if (nl + (1LL << h) - 1 <= r) {
            Value v = P::op(nodes[k].v, sm);
            if (g(v)) {
                sm = v;
                return nl - 1;
            }
            if (h == 0) return nl;
        }
        push(k, h);
        int64_t mid = nl + (1LL << (h - 1));
        int64_t res = min_left(nodes[k].c[1], h - 1, mid, r, g, sm);
        if (res != mid - 1) return res;
        return min_left(nodes[k].c[0], h - 1, nl, r, g, sm);
    }
};

} // namespace algo::ds
