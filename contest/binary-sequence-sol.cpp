#include <bits/stdc++.h>
#define int long long
using namespace std;
int MOD = 1e9 + 7;
int fastExp(int a, int b, int mod) {
    if (b == 0)
        return 1;
    else if (b == 1)
        return a % mod;
    else {
        int res = fastExp(a, b / 2, mod);
        res = res * res % mod;
        if (b % 2 == 1)
            res = res * a % mod;
        return res;
    }
}
signed main() {
    string num;
    cin >> num;
    int ctr = 1;
    int ans = 0;
    for (int i = 0; i < num.length(); i++) {
        if (num[i] == '1') {
            ans += (fastExp(3, num.length() - i - 1, MOD) % MOD * ctr) % MOD;
            ans %= MOD;
            ctr *= 2;
            ctr %= MOD;
        }
    }
    cout << ans << endl;
}
