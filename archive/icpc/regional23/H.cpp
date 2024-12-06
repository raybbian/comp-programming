#include <bits/stdc++.h>
using namespace std;

#define vt vector

int main() {
	cin.tie(0);
	ios::sync_with_stdio(0);
	int n; cin>>n;
	vt<int> v(n); for (int i=0; i<n; i++) cin>>v[i];
	int l=0, r=n-1;
	while (l<n && v[l]==0) l++;
	while (r>=0 && v[r]==0) r--;
	if (l>r) {
		cout << 0 << endl;
		return 0;
	}
	v.erase(v.begin()+r+1, v.end());
	v.erase(v.begin(), v.begin()+l);
	n=v.size();
	vt<vt<int>> dp(n, vt<int>(n, n));
	for (int i=0; i<n; i++) dp[i][i]=1;
	for (int i=n-1; i>=0; i--) {
		for (int j=i+1; j<n; j++) {
			for (int k=i; k<j; k++) {
				dp[i][j]=min(dp[i][j], dp[i][k]+dp[k+1][j]-(v[i]==v[j]));
			}
		}
	}
	cout << dp[0][n-1] << endl;
}
