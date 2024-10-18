#include <bits/stdc++.h>

#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);
    string m = inf.readString();
    inf.readEof();

    ensuref(!m.empty(), "M cannot be empty");
    ensuref(m[0] == '1', "M cannot have leading 0s") ;
    for (char c : m) {
        ensuref(c == '0' || c == '1', "M must be a binary string");
    }
}
