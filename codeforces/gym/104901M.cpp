#include <bits/stdc++.h>
using namespace std;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template <typename T> using oset = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define ll long long
#define ld long double

#define vt vector
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)(x).size())

struct pt {
    ld x, y;
};

int orientation(pt a, pt b, pt c) {
    ld v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // clockwise
    if (v > 0) return +1; // counter-clockwise
    return 0;
}

bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = orientation(a, b, c);
    return o < 0 || (include_collinear && o == 0);
}
bool collinear(pt a, pt b, pt c) { return orientation(a, b, c) == 0; }

pair<vt<pt>, vt<pt>> convex_hull(vector<pt> a, bool include_collinear = false) {
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.y, a.x) < make_pair(b.y, b.x);
    });
    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = orientation(p0, a, b);
        if (o == 0)
            return (p0.x-a.x)*(p0.x-a.x) + (p0.y-a.y)*(p0.y-a.y)
                   < (p0.x-b.x)*(p0.x-b.x) + (p0.y-b.y)*(p0.y-b.y);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)a.size()-1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin()+i+1, a.end());
    }

    vector<pt> st, nst;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size()-2], st.back(), a[i], include_collinear)) {
            nst.push_back(st.back());
            st.pop_back();
        }
        st.push_back(a[i]);
    }
    return {st, nst};
}

int main() {
    int n; cin>>n;
    vt<pt> v(n); for (int i=0; i<n; i++) cin>>v[i].x>>v[i].y;
    auto [st, nst]= convex_hull(v, false);
    int m1=sz(st), m2=sz(nst), ans=0;
    if (m2) {
        vt<vt<ld>> angles(m2, vt<ld>(m2 - 1));
        for (int i = 0; i < m2; i++) {
            for (int j = 0; j < i; j++) {
                angles[i][j] = atan2l(nst[j].y - nst[i].y, nst[j].x - nst[i].x);
            }
            for (int j = i + 1; j < m2; j++) {
                angles[i][j - 1] = atan2l(nst[j].y - nst[i].y, nst[j].x - nst[i].x);
            }
            sort(all(angles[i]));
        }
        for (int i=0; i<m1-1; i++) {
            for (int j=0; j<m2; j++) {
                ld a1=atan2l(st[i].y-nst[j].y, st[i].x-nst[j].x);
                ld a2=atan2l(st[i+1].y-nst[j].y, st[i+1].x-nst[j].x);
                if (a1>a2) {
                    auto it=lower_bound(all(angles[j]), a2);
                    if (it==angles[j].end() || *it>a1) ans++;
                } else {
                    auto it1=lower_bound(all(angles[j]), a2);
                    auto it2=lower_bound(all(angles[j]), a1);
                    if (it1==angles[j].end() && it2==angles[j].begin()) ans++;
                }
            }
        }
        for (int j=0; j<m2; j++) {
            ld a1=atan2l(st[m1-1].y-nst[j].y, st[m1-1].x-nst[j].x);
            ld a2=atan2l(st[0].y-nst[j].y, st[0].x-nst[j].x);
            if (a1>a2) {
                auto it=lower_bound(all(angles[j]), a2);
                if (it==angles[j].end() || *it>a1) ans++;
            } else {
                auto it1=lower_bound(all(angles[j]), a2);
                auto it2=lower_bound(all(angles[j]), a1);
                if (it1==angles[j].end() && it2==angles[j].begin()) ans++;
            }
        }
    }
    cout<<ans+1<<endl;
}