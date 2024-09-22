#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	int n = rnd.next(1, 2000);
	cout << n << ' ' << rnd.next(1, 100000) << ' ' << rnd.next(1, 100000) << '\n';
	for (int i = 0; i < n; i++) {
	    cout << rnd.next(1, 1000000000) << '\n';
	}
}
