#include <bits/stdc++.h>
using namespace std;

int main() {
#define int long long 
    int n;
    cin >> n;
    int a[2*n], b[2*n];
    for(int i = 0; i < 2*n; i++){
        cin >> a[i] >> b[i];
        if(a[i] > b[i]){
            int tmp = a[i];
            a[i] = b[i];
            b[i] = tmp;
        }
    }
    vector<pair<int, pair<int, int>>> v;
    for(int i = 0; i < 2*n; i++){
        v.push_back(make_pair(a[i] + b[i], make_pair(a[i], b[i])));

    }
    sort(v.begin(), v.end());
    int ans = 0;
    for(int i = 0; i < n; i++){
        ans -= v[i].second.first;
    }
    for(int i = n; i < 2*n; i++){
        ans += v[i].second.second;
    }
    cout << ans;
}