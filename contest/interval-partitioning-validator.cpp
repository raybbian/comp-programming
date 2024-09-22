#include <bits/stdc++.h>
#include "testlib.h"

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	int n = inf.readInt(1, 100'000, "n");
	inf.readEoln();

	for (int i = 0; i < n; i++) {
		int s = inf.readInt(0, 1'000'000'000, "s");
		inf.readSpace();
		int e = inf.readInt(0, 1'000'000'000, "e");
		inf.readEoln();

		ensuref(s < e, "Start time %d not less than end time %d", s, e);
	}

	inf.readEof();
}


