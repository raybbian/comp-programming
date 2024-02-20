//
// Created by rayb on 2/10/24.
//
#include <bits/stdc++.h>
using namespace std;

#define pii pair<int, int>

#define vt vector
#define pb push_back
#define sz(x) (int)(x).size()

vt<vt<pii>> adj;
vt<vt<int>> d;

int main() {
    int n, m, a, b, x, y, z; cin>>n>>m>>a>>b; --a, --b;
    adj.resize(n), d.assign(n, vt<int>(n));
    int total=0;
    for (int i=0; i<m; i++) {
        cin>>x>>y>>z; --x, --y;
        total+=z;
        d[x][y]=z;
        d[y][x]=z;
        adj[x].pb({y, z});
        adj[y].pb({x, z});
    }
    vt<int> dist(n, INT_MAX);
    vt<vt<int>> par(n);
    priority_queue<pii, vt<pii>, greater<pii>> pq;
    dist[a]=0; pq.push({0, a});
    while (sz(pq)) {
        auto [w, v]=pq.top(); pq.pop();
        if (w>dist[v]) continue;
        for (auto [u, c] : adj[v]) {
            if (dist[v]+c<dist[u]) {
                dist[u]=dist[v]+c;
                pq.push({dist[u], u});
                par[u].clear();
                par[u].pb(v);
            } else if (dist[v]+c==dist[u]) {
                par[u].pb(v);
            }
        }
    }
    queue<int> q; q.push(b);
    vt<bool> vis(n, 0);
    while (sz(q)) {
        int v=q.front(); q.pop();
        if (vis[v]) continue;
        vis[v]=1;
        for (auto p : par[v]) {
            total -= d[v][p];
            q.push(p);
        }
    }
    cout<<total<<endl;
}