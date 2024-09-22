#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
using namespace std;

const int MAX = 2000;
int N, P, Q, S, E[MAX];
vector<int> V;

bool check(int w) {
    int dp[S][P + 1], s = 0, b = 0;
    for (int i = 0; i < S; i++) fill(dp[i], dp[i] + P + 1, 0);
    dp[0][0] = 1;

    for (int i = 1; i < S; i++) {
        for (int j = 0; j <= P; j++) {
            while (s < i && V[i] - V[s] >= w) s++;
            while (b < i && V[i] - V[b] >= 2 * w) b++;
            if (j) {
                if (!s) dp[i][j] = 0;
                else if (!b) dp[i][j] = min(dp[s - 1][j - 1], 1);
                else dp[i][j] = min(dp[s - 1][j - 1], dp[b - 1][j] + 1);
            } else {
                if (!b) dp[i][0] = 1;
                else dp[i][0] = dp[b - 1][0] + 1;
            }
        }
    }
    return dp[S - 1][P] <= Q;
}


int main() {
ios_base::sync_with_stdio(0);
cin.tie(0);

    cin >> N >> P >> Q;
    for (int i = 0; i < N; i++) cin >> E[i];
    sort(E, E + N);
    V.push_back(E[0]);
    for (int i = 1; i < N; i++) if (E[i] != V[V.size() - 1]) V.push_back(E[i]);
    S = V.size();

    if (P + Q >= N) {
        cout << 1;
        return 0;
    }

    int l = 1, r = 1e9, m, ans;
    while (l <= r) {
        m = (l + r) / 2;
        if (check(m)) {
            ans = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    cout << ans;
    return 0;
}
