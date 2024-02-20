#include <bits/stdc++.h>
using namespace std;

#define f first
#define s second

void setIO(string name) {
	cin.tie(nullptr)->sync_with_stdio(false);
	if (name.length() > 0) {
		freopen((name + ".in").c_str(), "r", stdin);
		freopen((name + ".out").c_str(), "w", stdout);
	}
}

bool cmp(const pair<int, int> &a, const pair<int, int> &b) {
	return a.s < b.s;
}

int main() {
	setIO("");
	//k segments
	//k itself has to be a palindrome
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		int n, k, ans = 0;
		cin >> n >> k;
		string st;
		cin >> st;
		unordered_map<int, map<int, int>> info; //parity, count of letter, letter
		for (int j = 0; j < n; j++) {
			int parity = j % k;
			int letter = int(st[j]) - 97;
			info[parity][letter]++;
//			cout << parity << " " << st[j] << "\n";
		}
		for (int j = 0; j < (k + 1) / 2; j++) {
			char maxLetter = -1;
			int maxFreq = 0;
			if (j != (k - j - 1)) {
				for (int l = 0; l < 26; l++) {
					if (info[j][l] + info[k - j - 1][l] > maxFreq) {
						maxLetter = char(l + 97);
						maxFreq = info[j][l] + info[k - j - 1][l];
					}
				}
//				cout << "pos: " << j << "-" << (k - j - 1) << " " << maxLetter << " " << maxFreq << "\n";
				ans += 2 * n / k - maxFreq;
			} else { //middle letter
				for (int l = 0; l < 26; l++) {
					if (info[j][l] > maxFreq) {
						maxLetter = char(l + 97);
						maxFreq = info[j][l];
					}
				}
//				cout << "pos: " << j << "-" << (k - j - 1) << " " << maxLetter << " " << maxFreq << "\n";
				ans += n / k - maxFreq;
			}
//			cout << char(maxLetter + 97) << " " << maxFreq << " ";
		}
		cout << ans << "\n";;
	}
}