//
// Created by rayb on 2/3/24.
//
#include <bits/stdc++.h>
using namespace std;

#define vt vector
#define pb push_back
#define ll long long

int main() {
    int n, k; ll x, r, p; cin>>n>>x>>r>>p>>k;
    string s; cin>>s;
    vt<int> ri, pi;
    for (int i=0; i<n; i++) {
        if (s[i]=='1') ri.pb(i);
        else pi.pb(i);
    }
    int nr=ri.size(), np=pi.size();
    vt<ll> prer(nr+1), prerp(nr+1), prep(np+1);
    prer[0]=0, prep[0]=0;
    for (int i=0; i<nr; i++) prer[i+1]=prer[i]+(n-ri[i])*r;
    for (int i=0; i<nr; i++) prerp[i+1]=prerp[i]+(n-ri[i])*(r+p);
    for (int i=0; i<np; i++) prep[i+1]=prep[i]+(n-pi[i])*p;
    ll ans=LLONG_MIN;
    for (int i=0; i<min(nr, k+1); i++) {
        ans=max(ans, prerp[i]-prer[nr]+prep[min(k-i, ri[i]-i)]);
    }
    if (k>=nr) ans=max(ans, prerp[nr]+prep[k-nr]-prer[nr]);
    cout << ans+n*x << endl;
}