#include <bits/stdc++.h>
using namespace std;

#define vt vector
#define pii pair<int, int>
#define pb push_back

void solve() {
    int n; cin>>n;
    vt<int> v(n);
    vt<pii> ans;
    for (int i=0; i<n; i++) cin>>v[i];
    for (int i=0; i<n-1; i++) {
        for (int j=n-1; j>i; j--) {
            if (v[i]>v[j]) {
                sort(v.begin()+i, v.begin()+j+1);
                ans.pb({i+1, j+1});
            }
        }
    }
    assert(ans.size()<=n/2);
    cout<<ans.size()<<endl;
    for (auto a : ans) cout<<a.first<<" "<<a.second<<endl;
}

int main() {
    int t; cin>>t;
    for (int i=0; i<t; i++) solve();
}