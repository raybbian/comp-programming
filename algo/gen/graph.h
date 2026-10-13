#pragma once
#include "algo/common.h"
#include "algo/gen/random.h"

namespace algo::gen {

// Vertices are 1..n throughout, so edges print as read.
using edges = std::vector<std::pair<int, int>>;

// Random tree, each vertex's parent an earlier vertex. bias > 0 prefers recent
// parents (long paths), bias < 0 early ones (star-like).
//   tree(4);  // [(1, 2), (1, 3), (3, 4)]
inline edges tree(int n, int bias = 0) {
    edges e;
    for (int i = 2; i <= n; i++)
        e.emplace_back(num(1, i - 1, bias), i);
    return e;
}

// 1 - 2 - 3 - ... - n
//   path(4);  // [(1, 2), (2, 3), (3, 4)]
inline edges path(int n) {
    edges e;
    for (int i = 2; i <= n; i++)
        e.emplace_back(i - 1, i);
    return e;
}

// Every vertex attached to 1.
//   star(4);  // [(1, 2), (1, 3), (1, 4)]
inline edges star(int n) {
    edges e;
    for (int i = 2; i <= n; i++)
        e.emplace_back(1, i);
    return e;
}

// Path on 1..spine with the remaining vertices hanging off it.
//   caterpillar(5, 2);  // [(1, 2), (2, 3), (1, 4), (2, 5)]
inline edges caterpillar(int n, int spine) {
    assert(1 <= spine && spine <= n);
    edges e = path(spine);
    for (int i = spine + 1; i <= n; i++)
        e.emplace_back(num(1, spine), i);
    return e;
}

// Parent of i is i / 2.
//   binary_tree(4);  // [(1, 2), (1, 3), (2, 4)]
inline edges binary_tree(int n) {
    edges e;
    for (int i = 2; i <= n; i++)
        e.emplace_back(i / 2, i);
    return e;
}

// Extends e to m distinct pairs: rejection sampling with draw when sparse,
// otherwise a shuffle of every pair that each(f) yields.
template <typename D, typename E>
edges fill_pairs(edges e, int m, int64_t total, D draw, E each) {
    std::set<std::pair<int, int>> used(e.begin(), e.end());
    assert((int)e.size() <= m && m <= total);
    if (2 * int64_t(m) > total) {
        edges rest;
        each([&](int u, int v) {
            if (!used.count({u, v})) rest.emplace_back(u, v);
        });
        shuffle(rest);
        e.insert(e.end(), rest.begin(), rest.begin() + (m - e.size()));
    } else {
        while ((int)e.size() < m) {
            auto p = draw();
            if (used.insert(p).second) e.push_back(p);
        }
    }
    return e;
}

// Simple undirected graph with m edges, each stored as (u, v) with u < v.
//   graph(4, 3);        // [(1, 3), (2, 3), (1, 4)]
//   graph(4, 3, true);  // connected
inline edges graph(int n, int m, bool connected = false) {
    assert(!connected || m >= n - 1);
    return fill_pairs(
        connected ? tree(n) : edges{}, m, int64_t(n) * (n - 1) / 2,
        [&] {
            int u = num(1, n), v = num(1, n - 1);
            if (v >= u) v++;
            return std::pair{std::min(u, v), std::max(u, v)};
        },
        [&](auto f) {
            for (int u = 1; u <= n; u++) {
                for (int v = u + 1; v <= n; v++)
                    f(u, v);
            }
        });
}

// DAG with m edges u -> v, u < v. Stays acyclic under relabel.
//   dag(4, 3);  // [(1, 3), (2, 4), (1, 2)]
inline edges dag(int n, int m) {
    return graph(n, m);
}

// Bipartite graph: m edges between 1..l and l+1..l+r.
//   bipartite(2, 2, 3);  // [(1, 3), (2, 3), (2, 4)]
inline edges bipartite(int l, int r, int m) {
    return fill_pairs(
        {}, m, int64_t(l) * r,
        [&] { return std::pair{num(1, l), num(l + 1, l + r)}; },
        [&](auto f) {
            for (int u = 1; u <= l; u++) {
                for (int v = l + 1; v <= l + r; v++)
                    f(u, v);
            }
        });
}

// Random vertex labels, edge order and endpoint order.
// keep_root leaves vertex 1 as 1; directed keeps (u, v) as u -> v.
//   relabel(4, path(4));  // [(4, 2), (3, 1), (2, 3)]
inline edges relabel(int n, edges e, bool keep_root = false,
                     bool directed = false) {
    auto p = perm(n, 1);
    if (keep_root) std::swap(p[0], *std::find(p.begin(), p.end(), 1));
    for (auto &[u, v] : e) {
        u = p[u - 1], v = p[v - 1];
        if (!directed && chance(0.5)) std::swap(u, v);
    }
    shuffle(e);
    return e;
}

// Parent of every vertex 1..n with the tree rooted at root (root's is 0).
//   parents(4, tree(4));  // [_, 0, 1, 1, 3]
inline std::vector<int> parents(int n, const edges &e, int root = 1) {
    std::vector<std::vector<int>> adj(n + 1);
    for (auto [u, v] : e)
        adj[u].push_back(v), adj[v].push_back(u);
    std::vector<int> par(n + 1, -1), order{root};
    par[root] = root;
    for (size_t i = 0; i < order.size(); i++) {
        for (int v : adj[order[i]]) {
            if (par[v] == -1) par[v] = order[i], order.push_back(v);
        }
    }
    par[root] = 0;
    return par;
}

} // namespace algo::gen
