#include <bits/stdc++.h>
using namespace std;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template <typename T> using oset = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

#define ll long long

#define vt vector
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)(x).size())

struct FT {
    vector<ll> s;
    FT(int n) : s(n) {}
    void update(int pos, ll dif) { // a[pos] += dif
        for (; pos < sz(s); pos |= pos + 1) s[pos] += dif;
    }
    ll query(int pos) { // sum of values in [0, pos)
        ll res = 0;
        for (; pos > 0; pos &= pos - 1) res += s[pos-1];
        return res;
    }
    int lower_bound(ll sum) {// min pos st sum of [0, pos] >= sum
        // Returns n if no sum is >= sum, or -1 if empty sum is.
        if (sum <= 0) return -1;
        int pos = 0;
        for (int pw = 1 << 25; pw; pw >>= 1) {
            if (pos + pw <= sz(s) && s[pos + pw-1] < sum)
                pos += pw, sum -= s[pos-1];
        }
        return pos;
    }
};
void solve() {
    int n; ll k; cin>>n>>k;
    vt<ll> v(n); vt<pll> idx(n); for (int i=0; i<n; i++) cin>>v[i], idx[i]={v[i]-i, i};
    sort(all(idx));
    FT ft(n);
    int st=lower_bound(all(idx), make_pair(v[0], 0ll)) - idx.begin();
    oset<int> os; os.insert(st); ft.update(st, v[0]);
    int l=0, r=0, ans=1;
    while (r<n) {
        auto mid = *os.find_by_order(sz(os) / 2);
        ll c=(idx[mid].first * (sz(os)/2+1) - ft.query(mid + 1)) + (ft.query(n) - ft.query(mid + 1) - idx[mid].first * (sz(os) - sz(os) / 2 - 1));
        if (c<=k) {
            ans = max(ans, r - l + 1);
            if (++r<n) {
                int i = lower_bound(all(idx), make_pair(v[r] - r, (ll)r)) - idx.begin();
                ft.update(i, v[r] - r);
                os.insert(i);
            }
        } else {
            int i=lower_bound(all(idx), make_pair(v[l]-l, (ll)l))-idx.begin();
            ft.update(i, -v[l]+l);
            os.erase(i);
            l++;
        }
    }
    cout<<ans<<endl;
}

int main() {
    int t; cin>>t;
    for (int i=0; i<t; i++) solve();
}