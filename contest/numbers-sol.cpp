#include <bits/stdc++.h>
using namespace std;

//#define int long long
#define dbg(v)                                                                 \
cout << "Line(" << __LINE__ << ") -> " << #v << " = " << (v) << endl;

typedef long long ll;
typedef pair<int, int> pi;
typedef vector<pi> vpi;
typedef vector<int> vi;
#define INF 1000000000

ll memo[11][11][20];
string a, b;

ll dp(int prev1, int prev2, int index, int tight, int leading, string N) {
//    cout << prev1 << " " << prev2 << " " << index << " " << tight << " " << leading << endl;
    if (index == 0) return 1;
    if (memo[prev1][prev2][index] != -1 && !tight && !leading) return memo[prev1][prev2][index];

    int ub = tight ? N[N.size() - index] - '0' : 9;
    ll ans = 0;

    for (int a = 0; a <= ub; a++) {
        if ((prev1 == a && !(prev1 == 0 && leading))|| (prev2 == a && !(prev2 == 0 && leading))) continue;
        ans += dp(prev2, a, index-1, tight & (a == ub), ((!prev2 || prev2 == 10) & leading), N);
    }
    if (!tight && !leading) memo[prev1][prev2][index] = ans;
    return ans;
}

ll naive(int a, int b) {
    ll ans = 0;

    for (int i = a; i <= b; i++) {
            int increment = 1;
            string s = to_string(i);
            for (int j = 1; j < s.size(); j++) {
                if (s[j-1] == s[j]) {
                    increment = 0;
                    break;
                }
                if (j == 1) continue;
                if (s[j-2] == s[j]) {
                    increment = 0;
                    break;
                }
            }
            ans += increment;
        }
    return ans;
}

int32_t main() {
ios_base::sync_with_stdio(false);
cin.tie(NULL);
cout.tie(NULL);

// code here
    ll x, y;
    cin >> x >> y;
    memset(memo, -1, sizeof(memo));
    a = to_string(x-1);
    b = to_string(y);

    ll ans = 0;
    ans += dp(10, 10, b.size(), 1, 1, b);
//        cout << ans << endl;
    ans -= dp(10, 10, a.size(), 1, 1, a);
    cout << ans;
//        cout << naive(x, y) << endl;
}
