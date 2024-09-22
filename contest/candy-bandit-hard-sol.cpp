#include <deque>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int t;
    cin >> t;
    for (int tt = 0; tt < t; tt++) {

        int n, k;
        cin >> n >> k;

        vector<int> b(n + 1);

        for (int i = 0; i < n; ++i) {
            cin >> b[i];
        }

        b[n] = 0;

        int c = 0;
        deque<int> dq;

        for (int i = 0; i <= n; ++i) {

            if (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }

            while (!dq.empty() && b[dq.back()] >= b[i]) {
                dq.pop_back();
            }

            dq.push_back(i);

            if (i >= k - 1) {
                c = max(c, b[dq.front()]);
            }
        }

        cout << c << endl;
    }
    return 0;
}
