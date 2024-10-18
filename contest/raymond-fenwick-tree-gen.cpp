#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    /* Read number from command line. */
    int n = opt<int>(1);
    int q = opt<int>(2);
    int max_a = opt<int>(3);

    cout << n << ' ' << q << '\n';

    for (int i = 0; i < n; i++) {
        int num = rnd.next(1, max_a);
        cout << num << " \n"[i == n - 1];
    }

    for (int i = 0; i < q - 1; i++) {
        int typ = rnd.next(1, 2);
        if (typ == 1) {
            int x = rnd.next(1, n);
            int v = rnd.next(1, max_a);
            cout << typ << ' ' << x << ' ' << v << '\n';
        } else {
            int l = rnd.next(1, n);
            int r = rnd.next(l, n);
            cout << typ << ' ' << l << ' ' << r << '\n';
        }
    }
    int l = rnd.next(1, n);
    int r = rnd.next(l, n);
    cout << 2 << ' ' << l << ' ' << r << '\n';
}
