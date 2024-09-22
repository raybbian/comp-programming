#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define MAXN (1000005)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
string target = "";
ll memo[18][2][2];
ll dp(ll digit, bool prev_1, bool bounded) {
    if (digit == target.size() - 1)
        return memo[digit][prev_1][bounded] = 1;
    if (memo[digit][prev_1][bounded] != -1)
        return memo[digit][prev_1][bounded];
    ll ans = 0;
    if (bounded) {
        if (target[digit + 1] != '4' && !(target[digit + 1] == '3' && prev_1))
            ans += dp(digit + 1, target[digit + 1] == '1', bounded);
        ll multiply = ll(target[digit + 1] - '0');
        if (target[digit + 1] > '1')
            ans += dp(digit + 1, 1, 0), multiply--;
        if (target[digit + 1] > '4')
            multiply--;
        if (target[digit + 1] > '3' && prev_1)
            multiply--;
        ans += multiply * dp(digit + 1, 0, 0);
    } else {
        ans += dp(digit + 1, 1, 0);
        if (prev_1)
            ans += 7 * dp(digit + 1, 0, 0);
        else
            ans += 8 * dp(digit + 1, 0, 0);
    }
    memo[digit][prev_1][bounded] = ans;
    return memo[digit][prev_1][bounded];
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    ll N;
    cin >> N;
    memset(memo, -1, sizeof(memo));
    for (ll i = 0; i < N; i++) {
        ll type, X;
        cin >> type >> X;
        if (type == 1) {
            target = to_string(X);
            target = '0' + target;
            memset(memo, -1, sizeof(memo));
            bool ok = 1;
            for (ll i = 0; i < target.size(); i++) {
                if (target[i] == '4')
                    ok = 0;
                if (i + 1 < target.size() && target[i] == '1' && target[i + 1] == '3')
                    ok = 0;
            }
            if (!ok)
                cout << -1 << '\n';
            else
                cout << dp(0, 0, 1) - 1 << '\n';
        } else if (type == 2) {
            ll high = 1e17;
            ll low = 0;
            while (high - low > 1) {
                ll mid = (high + low) / 2;
                target = to_string(mid);
                target = '0' + target;
                memset(memo, -1, sizeof(memo));
                if (dp(0, 0, 1) - 1 >= X) {
                    high = mid;
                } else {
                    low = mid;
                }
            }
            cout << high << '\n';
        }
    }
}
