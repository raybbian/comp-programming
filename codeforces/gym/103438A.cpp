#include <bits/stdc++.h>
using namespace std;

#define vt vector
#define pb push_back
#define pii pair<int, int>
#define ll long long

int main() {
    int n; cin>>n;
    string s1, s2; cin>>s1>>s2;
    vt<pii> v;
    for (int i=0; i<n; i++) {
        if (s1[i]!=s2[i]) v.pb({i, s1[i]<s2[i]});
    }
    ll ans=0;
    for (int i=0; i<n; i++) {
        int idx=lower_bound(v.begin(), v.end(), make_pair(i, 0))-v.begin();
        if (idx==v.size()) break;
        if (v[idx].second) {
            ans+=n-v[idx].first;
        }
    }
    cout << ans << endl;
}