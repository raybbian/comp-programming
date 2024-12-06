#include <bits/stdc++.h>
using namespace std;

#define ll long long

void solve() {
	ll n, k, a, b, ans=0; cin>>n>>k>>a>>b;
	if (((b-a)*n)/a==0) {
		cout << -1 << endl;
		return;
	}
	while (k>0) {
		ll d=(b-a)*n;
		ll imp=d/a;
		d=a-(d%a);
		ll steps;
		if (imp>=d || b-a>=d) steps=1;
		else steps=d%(imp*(b-a)) ? d/(imp*(b-a))+1 : d/(imp*(b-a));
		n+=steps*imp;
		ans+=min(steps, k%imp ? k/imp+1 : k/imp);
		k-=steps*imp;
	}
	cout << ans << endl;
}


int main() {
	int t; cin>>t;
	for (int i=0; i<t; i++) solve();
}
