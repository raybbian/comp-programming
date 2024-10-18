#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);

    /* Read number from command line. */
    int n = opt<int>(1);
    bool has = false;
    for(int i = 31; i >= 0; i--) {
        if (n & (1 << i)) {
            has = true;
        }
        if (has) {
            cout << !!(n & (1 << i));
        }
    }
    cout << '\n';
}

