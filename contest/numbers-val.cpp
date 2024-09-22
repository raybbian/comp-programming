#include "testlib.h"

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	long long a = inf.readLong(0, 1'000'000'000'000'000'000LL, "a");
	inf.readSpace();
	long long b = inf.readLong(0, 1'000'000'000'000'000'000LL, "b");
	ensure(a <= b);
	inf.readEoln();
	inf.readEof();
}


