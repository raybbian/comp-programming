#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    /* Read number from command line. */
    int n = opt<int>(1);
    cout << '1';
    for(int i = 1; i < n; i++) {
        cout << rnd.next(0, 1);
    }
    cout << '\n';
}
