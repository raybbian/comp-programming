#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    int n = atoi(argv[1]);
    long long maxa = atoll(argv[2]);

    cout << n << '\n';
    for (int i = 0; i < n; i++) {
	long long a = rnd.next(1LL, maxa);
	int typ = rnd.next(1, 2);
	cout << typ << ' ' << a << '\n';
    }   
}
