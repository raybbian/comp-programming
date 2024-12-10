#include "../common.h"
#include "../utils/bits.h"

// TODO: replace with RMQ

namespace algo::graph {

struct lca {
    // Note that adj must be a tree. Don't forget to set root!
    lca(const std::vector<std::vector<int>> &adj, int root = 0)
        : n(sz(adj)), l(utils::lg2(n) + 1), timer(0), tin(n), tout(n),
          up(n, std::vector<int>(l + 1, root)) {
        dfs(root, root, adj);
    }
    // Returns true if u is an ancestor of v
    bool is_ancestor(int u, int v) {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }
    // Lowest common ancestor of u, v
    int par(int u, int v) {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;
        for (int i = l; i >= 0; i--)
            if (!is_ancestor(up[u][i], v)) u = up[u][i];
        return up[u][0];
    }

private:
    int n, l, timer;
    std::vector<int> tin, tout;
    std::vector<std::vector<int>> up;

    void dfs(int v, int p, const std::vector<std::vector<int>> &adj) {
        tin[v] = ++timer;
        up[v][0] = p;
        for (int i = 1; i <= l; i++)
            up[v][i] = up[up[v][i - 1]][i - 1];
        for (int u : adj[v])
            if (u != p) dfs(u, v, adj);
        tout[v] = ++timer;
    }
};

}; // namespace algo::graph
