#include "testlib.h"

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);
    int n = inf.readInt(1, 100'000);
    inf.readEoln();
    for (int i = 0; i < n; i++) {
	inf.readInt(1, 2);
	inf.readSpace();
	inf.readLong(1LL, 10'000'000'000'000'000LL);
	inf.readEoln();
    }
    inf.readEof();
}


