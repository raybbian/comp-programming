#include <bits/stdc++.h>
using namespace std;

#define f first
#define s second

const int MOD = 1e9 + 7;

void setIO(string name) {
	cin.tie(nullptr)->sync_with_stdio(false);
	if (name.length() > 0) {
		freopen((name + ".in").c_str(), "r", stdin);
		freopen((name + ".out").c_str(), "w", stdout);
	}
}

long long mod(long long a, long long b) {
	return (a % b + b) % b;
}

long long modpow(long long base, long long exp, long long modulus) {
	base %= modulus;
	long long result = 1;
	while (exp > 0) {
		if (exp & 1) result = (result * base) % modulus;
		base = (base * base) % modulus;
		exp >>= 1;
	}
	return result;
}

int main() {
	setIO("");
	int n;
	cin >> n;
	long long k = modpow(3, 3 * n, MOD);
	long long l = modpow(7, n, MOD);
	cout << mod(k - l, MOD);
}


/*
 * 3 * 3 * 3 = 27 total ways to distribute coins
 * {2, 2, 2}, {3, 2, 1}, {3, 1, 2}, {2, 3, 1}, {2, 1, 3}, {1, 3, 2}, {1, 2, 3} ways to distribute = 7
 *
 *
 * one triangle is not 6 = 20
 *
 * next one is either six or not six = 7 ways, 20 ways
 *
 *
 * both triangles not 6 = 20 * 20 * 1
 * one triangle 6       = 20 *  7 * 2 (choose which triangle is 6)
 *
 *
 * 3 triangles not 6 = 20 * 20 * 20 * 1 = 8000
 * one triangle 6    = 20 * 20 *  7 * 3 = 8400
 * two triangles 6   = 20 * 7  *  7 * 3 = 2940
 *
 *
 * */