#pragma once
#include "algo/common.h"
#include "algo/ds/sparse_table.h"
#include "algo/utils/bits.h"

namespace algo::graph {

struct lca {
    // Note that adj must be a tree. Don't forget to set root!
    lca(const std::vector<std::vector<int>> &adj, int root = 0)
        : n(sz(adj)), height(n), first(n), st(2 * n, [&](int a, int b) {
              return height[a] < height[b] ? a : b;
          }) {
        euler.reserve(2 * n);
        dfs(root, root, 0, adj);
        st.init(euler);
    }
    // Lowest common ancestor of u, v
    int par(int u, int v) {
        int l = first[u], r = first[v];
        if (l > r) std::swap(l, r);
        return st.query(l, r);
    }

private:
    int n;
    std::vector<int> height, euler, first;
    ds::sparse_table<int> st;

    void dfs(int v, int p, int h, const std::vector<std::vector<int>> &adj) {
        height[v] = h;
        first[v] = sz(euler);
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
