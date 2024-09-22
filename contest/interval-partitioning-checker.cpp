#include <bits/stdc++.h>
#include "testlib.h"

int main(int argc, char* argv[]) {
	registerTestlibCmd(argc, argv);
	
	int n = inf.readInt();

	int pans = ouf.readInt(1, n, "min lecture theaters");

	int jans = ans.readInt();
	
	if (pans == jans) {
		quitf(_ok, "Minimum number of lecture halls matches jury.");
	} else {
		quitf(_wa, "Incorrect number of lecture halls: expected = %d, found = %d", jans, pans);
	}
}
