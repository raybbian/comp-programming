#include "../common.h"
#include "../utils/bits.h"

// TODO: replace with RMQ

namespace algo::graph {

struct lca {
    // Note that adj must be a tree. Don't forget to set root!
    lca(const std::vector<std::vector<int>> &adj, int root = 0)
        : timer(0), L(utils::lg2(sz(adj)) + 1), tin(sz(adj)), tout(sz(adj)) {
        up.assign(sz(adj), std::vector<int>(L + 1, -1));
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
        for (int i = L; i >= 0; i--)
            if (!is_ancestor(up[u][i], v)) u = up[u][i];
        return up[u][0];
    }

private:
    int timer;
    int L;
    std::vector<std::vector<int>> up;
    std::vector<int> tin;
    std::vector<int> tout;

    void dfs(int v, int p, const std::vector<std::vector<int>> &adj) {
        tin[v] = ++timer;
        up[v][0] = p;
        for (int i = 1; i <= L; i++)
            up[v][i] = up[up[v][i - 1]][i - 1];
        for (int u : adj[v])
            if (u != p) dfs(u, v, adj);
        tout[v] = ++timer;
    }
};

}; // namespace algo::graph
