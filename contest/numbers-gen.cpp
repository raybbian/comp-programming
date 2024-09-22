#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	long long b = rnd.next(1LL, 1'000'000'000'000'000'000LL);
	long long a = rnd.next(1LL, b);
	cout << a << ' ' << b << '\n';
}
