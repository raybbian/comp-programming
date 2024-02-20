#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

typedef long long ll;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for (int ti = 0; ti < t; ++ti) {
        ll l, r, k;
        cin >> l >> r >> k;
        ll count = 0;
        for (ll i = 1; i * i <= k; ++i) {
            ll j = k / i;
            if (i * j == k && __gcd(i, j) == 1) {
                count += max(floor(double(r) / max(i, j)) - ceil(double(l) / min(i, j)) + 1, double(0));
            }
        }
        cout << count << '\n';
    }
}