#include <bits/stdc++.h>
using namespace std;

#define vt vector
#define pb push_back
#define pf pop_front

int main() {
	string s; cin>>s;
	int n=s.size(), ans=1;
	vt<int> cnt(n/3, 3);
	vt<deque<int>> need(3);
	int l=0, r=0;
	for (int i=0; i<n; i++) {
		int idx=s[i]-'A';
		if (need[idx].size()) {
			if (--cnt[need[idx].front()]==0) l++;
			need[idx].pop_front();
		} else {
			cnt[r]--;
			for (int i=0; i<3; i++) if (i!=idx) need[i].pb(r);
			r++;
		}
		ans=max(ans, r-l);
	}
	cout << ans << endl;
}
