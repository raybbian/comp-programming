#include <bits/stdc++.h>

using namespace std;

bool dp[1001][1001];

int main() {
#define int long long
    int c, n; cin >> c >> n;
    dp[0][0] = 1;
    int sum = 0;
    for(int i = 0; i < n; i++){
        int a; cin >> a;
        for(int j = min(c, sum); j >= 0; j--){
            for(int k = min(c, sum - j); k >= 0; k--){
                if(dp[j][k]) {
                    if (j + a <= c) {
                        dp[j + a][k] = 1;
                    }
                    if (k + a <= c) {
                        dp[j][k + a] = 1;
                    }
                }
            }
        }
        sum += a;
    }

    int mx = 0;
    pair<int, int> p = {0, 0};

    for(int i = 0; i <= c; i++){
        for(int j = 0; j <= i; j++){
            if(dp[i][j]) {
                if(mx < i + j) {
                    mx = i + j;
                    p = {i, j};
                }
                else if(mx == i + j) {
                    if(abs(p.first - p.second) > abs(i - j)) {
                        p = {i, j};
                    }
                }
            }
        }
    }

    cout << p.first << " " << p.second;
}