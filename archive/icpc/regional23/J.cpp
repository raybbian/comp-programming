#include <bits/stdc++.h>
using namespace std;

int main() {
	cout << 2 * 33333 + 2 << "\n";
	cout.flush();

	set<int, bool> guessed;
	guessed[0] = true;
	guessed[100000] = true;
	while(true) {
		int guess;
		cin >> guess;
		if (guess == -1) {
			break;
		}
		if (guessed.count(guess - 1) || guessed.count(guess + 1)) {
			cout << -1 << "\n";
			cout.flush();
		} else {
			cout << 
		}
	}
}
