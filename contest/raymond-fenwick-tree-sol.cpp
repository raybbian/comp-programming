#include <bits/stdc++.h>
#define int long long
#define pii pair<int, int>
#define ti3 tuple<int, int, int>

using namespace std;
struct node {
    vector<pii> pref, suff;
    int s, e, m, v;
    node *l, *r;
    node(int s, int e) : s(s), e(e), m((s + e) / 2), v(0) {
        if (s != e) {
            l = new node(s, m);
            r = new node(m + 1, e);
        }
        // dummy values
        pref.push_back({1, 1});
        suff.push_back({1, 1});
    }
    tuple<vector<pii>, vector<pii>, int> mergeChild(int lans, vector<pii> &lpref,
                                                    vector<pii> &lsuff, int rans,
                                                    vector<pii> &rpref, vector<pii> &rsuff) {
        int curr = lans + rans;
        for (int i = 0; i < lsuff.size(); i++) {
            for (int j = 0; j < rpref.size(); j++) {
                if (__gcd(lsuff[i].first, rpref[j].first) > 1)
                    curr += lsuff[i].second * rpref[j].second;
                else
                    break;
            }
        }
        vector<pii> newpref;
        for (auto i : lpref)
            newpref.push_back(i);
        for (auto [a, b] : rpref) {
            int newgcd = __gcd(a, newpref.back().first);
            if (newgcd == newpref.back().first)
                newpref.back().second += b;
            else
                newpref.push_back({newgcd, b});
        }
        vector<pii> newsuff;
        for (auto i : rsuff)
            newsuff.push_back(i);
        for (auto [a, b] : lsuff) {
            int newgcd = __gcd(a, newsuff.back().first);
            if (newgcd == newsuff.back().first)
                newsuff.back().second += b;
            else
                newsuff.push_back({newgcd, b});
        }
        return tuple<vector<pii>, vector<pii>, int>(newpref, newsuff, curr);
    }
    void upd(int x, int val) {
        if (s == e) {
            pref.clear();
            suff.clear();
            pref.push_back({val, 1});
            suff.push_back({val, 1});
            v = val != 1;
            return;
        } else if (x <= m) {
            l->upd(x, val);
        } else {
            r->upd(x, val);
        }
        auto sth = mergeChild(l->v, l->pref, l->suff, r->v, r->pref, r->suff);
        pref = get<0>(sth);
        suff = get<1>(sth);
        v = get<2>(sth);
    }

    tuple<vector<pii>, vector<pii>, int> query(int x, int y) {
        if (s == x && e == y)
            return tuple<vector<pii>, vector<pii>, int>(pref, suff, v);
        if (y <= m)
            return l->query(x, y);
        else if (x > m)
            return r->query(x, y);
        auto [a1, b1, c1] = l->query(x, m);
        auto [a2, b2, c2] = r->query(m + 1, y);
        return mergeChild(c1, a1, b1, c2, a2, b2);
    }

} *root;

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    root = new node(0, n - 1);
    for (int i = 0; i < n; i++) {
        int g;
        cin >> g;
        root->upd(i, g);
    }
    while (q--) {
        int t, a, b;
        cin >> t >> a >> b;
        if (t == 2) {
            cout << get<2>(root->query(a - 1, b - 1)) << '\n';
        } else {
            root->upd(a - 1, b);
        }
    }
}
