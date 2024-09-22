#include "testlib.h"

int main(int argc, char* argv[]) {
	registerValidation(argc, argv);
	int n = inf.readInt(1, 2000, "n");
	inf.readSpace();
	int p = inf.readInt(1, 100000, "p");
	inf.readSpace();
	int q = inf.readInt(1, 100000, "q");
	inf.readEoln();

	for (int i = 0; i < n; i++) {
	    inf.readInt(1, 1000000000, "a");
	    inf.readEoln();
	}

	inf.readEof();
}


