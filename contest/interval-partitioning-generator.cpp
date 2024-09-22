#include <bits/stdc++.h>
#include "testlib.h"

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	int n = atoi(argv[1]);
	std::cout << n << '\n';
	for(int i = 0; i < n; i++) {
		int s = rnd.next(n - 1);
		int e = rnd.next(s + 1, n);
		std::cout << s << ' ' << e << '\n';
	}
}

