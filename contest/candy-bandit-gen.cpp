#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	int t = atoi(argv[1]);
	int min_n = atoi(argv[2]);
	int max_n = atoi(argv[3]);
	int max_k = atoi(argv[4]);
	int max_a = atoi(argv[5]);

	cout << t << '\n';
	for (int i = 0; i < t; i++) {
		int n = rnd.next(min_n, max_n);
		int k = rnd.next(1, max_k);
		cout << n << ' ' << k << '\n';
		for (int j = 0; j < n; j++) {
			cout << rnd.next(1, max_a) << " \n"[j == n - 1];
		}
	}
}
