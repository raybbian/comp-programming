#pragma once
#include "algo/common.h"
#include "algo/ds/sparse_table.h"
#include "algo/utils/bits.h"

namespace algo::graph {

struct lca {
    // Note that adj must be a tree. Don't forget to set root!
    lca(const std::vector<std::vector<index_t>> &adj, index_t root = 0)
        : n((index_t)adj.size()), height(n), first(n), st(2 * n, {&height}) {
        euler.reserve(2 * n);
        dfs(root, root, 0, adj);
        st.init(euler);
    }
    // Lowest common ancestor of u, v
    index_t par(index_t u, index_t v) {
        index_t l = first[u], r = first[v];
        if (l > r) std::swap(l, r);
        return st.query(l, r);
    }

private:
    // The table is built before dfs fills height, so the op has to read it
    // where it lives rather than close over a copy.
    struct by_height {
        const std::vector<index_t> *height;
        index_t operator()(index_t a, index_t b) const {
            return (*height)[a] < (*height)[b] ? a : b;
        }
    };

    index_t n;
    std::vector<index_t> height, euler, first;
    ds::sparse_table<index_t, by_height> st;

    void dfs(index_t v, index_t p, index_t h,
             const std::vector<std::vector<index_t>> &adj) {
        height[v] = h;
        first[v] = (index_t)euler.size();
        euler.push_back(v);
        for (index_t u : adj[v]) {
            if (u != p) {
                dfs(u, v, h + 1, adj);
                euler.push_back(v);
            }
        }
    }
};

}; // namespace algo::graph
