#pragma once
#include "algo/common.h"

namespace algo::ds {

// Binary trie over B-bit values, high bit first.
//   xor_trie<30> t;
//   t.insert(x);
//   t.erase(x);           // must be present
//   t.max_xor(x);         // max of x ^ y over inserted y; nonempty
//   t.min_xor(x);
//   t.count_less(x, k);   // number of inserted y with x ^ y < k
template <int B>
struct xor_trie {
    using T = std::conditional_t<(B <= 32), uint32_t, uint64_t>;

    xor_trie() : nodes(1) {
    }
    int size() const {
        return nodes[0].cnt;
    }
    bool empty() const {
        return size() == 0;
    }
    void insert(T x) {
        int u = 0;
        nodes[u].cnt++;
        for (int b = B - 1; b >= 0; b--) {
            int &v = nodes[u].next[bit(x, b)];
            if (v == -1) {
                v = (int)nodes.size();
                nodes.emplace_back();
            }
            u = nodes[u].next[bit(x, b)];
            nodes[u].cnt++;
        }
    }
    void erase(T x) {
        int u = 0;
        assert(count(x) > 0);
        nodes[u].cnt--;
        for (int b = B - 1; b >= 0; b--) {
            u = nodes[u].next[bit(x, b)];
            nodes[u].cnt--;
        }
    }
    int count(T x) const {
        int u = 0;
        for (int b = B - 1; b >= 0 && u != -1; b--)
            u = nodes[u].next[bit(x, b)];
        return u == -1 ? 0 : nodes[u].cnt;
    }
    T max_xor(T x) const {
        return best_xor(x, 1);
    }
    T min_xor(T x) const {
        return best_xor(x, 0);
    }
    int count_less(T x, T k) const {
        if constexpr (B < std::numeric_limits<T>::digits)
            if (k >> B) return size();
        int u = 0, res = 0;
        for (int b = B - 1; b >= 0 && u != -1; b--) {
            int xb = bit(x, b);
            if (bit(k, b)) {
                res += cnt(nodes[u].next[xb]);
                u = nodes[u].next[xb ^ 1];
            } else {
                u = nodes[u].next[xb];
            }
        }
        return res;
    }
    friend std::ostream &operator<<(std::ostream &os, const xor_trie &t) {
        bool first = true;
        auto dfs = [&](auto &self, int u, int b, T val) -> void {
            if (t.nodes[u].cnt == 0) return;
            if (b < 0) {
                if (!first) os << ", ";
                first = false;
                os << val << ": " << t.nodes[u].cnt;
                return;
            }
            for (int c = 0; c < 2; c++) {
                int v = t.nodes[u].next[c];
                if (v != -1) self(self, v, b - 1, val | (T(c) << b));
            }
        };
        os << "{";
        dfs(dfs, 0, B - 1, 0);
        return os << "}";
    }

private:
    struct node {
        std::array<int, 2> next{-1, -1};
        int cnt = 0;
    };
    std::vector<node> nodes;

    static int bit(T x, int b) {
        return (int)(x >> b & 1);
    }
    int cnt(int u) const {
        return u == -1 ? 0 : nodes[u].cnt;
    }
    T best_xor(T x, int want) const {
        assert(!empty());
        int u = 0;
        T res = 0;
        for (int b = B - 1; b >= 0; b--) {
            int c = bit(x, b) ^ want;
            if (cnt(nodes[u].next[c]) == 0) c ^= 1;
            res |= T(c ^ bit(x, b)) << b;
            u = nodes[u].next[c];
        }
        return res;
    }
};

} // namespace algo::ds
