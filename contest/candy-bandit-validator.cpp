#include "testlib.h"

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);

	int testCount = inf.readInt(1, 100, "t");
	inf.readEoln();
	int total_n = 0;
	for (int i = 0; i < testCount; i++) {
		setTestCase(i + 1);
		int n = inf.readInt(1, 500'000, "n");
		total_n += n;
		inf.readSpace();
		inf.readInt(1, 10, "k");
		inf.readEoln();

		inf.readInts(n, 0, 1'000'000'000, "a");
		inf.readEoln();
	}
	ensure(total_n <= 500'000);

	inf.readEof();
}
