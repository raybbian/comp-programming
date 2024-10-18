#include <bits/stdc++.h>

#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 100'000);
    inf.readSpace();
    int q = inf.readInt(1, 100'000);
    inf.readEoln();

    inf.readInts(n, 1, 1'000'000'000);
    inf.readEoln();

    int cnt = 0;
    for(int i = 0; i < q; i++) {
        int t = inf.readInt(1, 2);
        inf.readSpace();
        if (t == 1) {
            inf.readInt(1, n);
            inf.readSpace();
            inf.readInt(1, 1'000'000'000);
            inf.readEoln();
        } else {
            cnt++;
            int l = inf.readInt(1, n);
            inf.readSpace();
            int r = inf.readInt(1, n);
            inf.readEoln();
            ensuref(l <= r, "Bad bounds!");
        }
    }

    inf.readEof();
    ensuref(cnt > 0, "Must be at least one instruction of query type");
}
