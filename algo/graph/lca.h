#pragma once
#include "algo/common.h"
#include "algo/ds/sparse_table.h"
#include "algo/utils/bits.h"

namespace algo::graph {

// Lowest common ancestor on a tree. O(n log n) build, O(1) query.
//   lca t(adj, root);
//   t.par(u, v);
struct lca {
    lca(const std::vector<std::vector<int>> &adj, int root = 0)
        : n((int)adj.size()), height(n), first(n), st(2 * n, {&height}) {
        euler.reserve(2 * n);
        dfs(root, root, 0, adj);
        st.init(euler);
    }
    // Lowest common ancestor of u and v
    int par(int u, int v) {
        int l = first[u], r = first[v];
        if (l > r) std::swap(l, r);
        return st.query(l, r);
    }

private:
    // Of two nodes, picks the shallower. Holds a pointer since height is
    // filled after st is constructed.
    struct by_height {
        const std::vector<int> *height;
        int operator()(int a, int b) const {
            return (*height)[a] < (*height)[b] ? a : b;
        }
    };

    int n;
    std::vector<int> height, euler, first;
    ds::sparse_table<int, by_height> st;

    void dfs(int v, int p, int h,
             const std::vector<std::vector<int>> &adj) {
        height[v] = h;
        first[v] = (int)euler.size();
        euler.push_back(v);
        for (int u : adj[v]) {
            if (u != p) {
                dfs(u, v, h + 1, adj);
                euler.push_back(v);
            }
        }
    }
};

}; // namespace algo::graph
