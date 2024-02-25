#include <bits/stdc++.h>
using namespace std;

int main() {
	//cin.tie(0);
	//ios::sync_with_stdio(false);

	int r, c;
	cin >> r >> c;
	vector<vector<int>> grid(r + 1, vector<int>(c + 1));
	for(int i = 1; i <= r; i++) {
		string s;
		cin >> s;
		for(int j = 1; j <= c; j++) {
			grid[i][j] = s[j - 1] - '0';
		}
	}

	//num_row needs to be number of 0's
	auto work = [](vector<vector<int>> &mat) {
		int num_r = mat.size() - 1;
		int num_c = mat[0].size() - 1;
		vector<vector<int>> pref_row(num_r + 1, vector<int>(num_c + 1));
		vector<vector<int>> pref_col(num_r + 1, vector<int>(num_c + 1));
		for(int i = 1; i <= num_r; i++) {
			for(int j = 1; j <= num_c; j++) {
				pref_row[i][j] += pref_row[i][j - 1] + (mat[i][j] == 0);
				pref_col[i][j] += pref_col[i - 1][j] + mat[i][j];
			}
		}


		vector<vector<vector<int>>> dp(num_c + 1, vector<vector<int>>(num_c + 1, vector<int>(num_r + 1, (int)1e9)));
		dp[0][0][0] = 0;
		dp[0][1][0] = 0;
		for(int l = 1; l <= num_c; l++) {
			for(int r = l; r <= num_c; r++) {
				for(int row = 1; row <= num_r; row++) {
					int up_l = dp[l - 1][r][row] + pref_col[num_r][l] - pref_col[row][l];
					int up_r = dp[l][r - 1][row] + pref_col[row - 1][r] - pref_col[0][r];
					int up_row = dp[l][r][row - 1] + pref_row[row][r] - pref_row[row][l - 1];
					dp[l][r][row] = min({up_l, up_r, up_row});
				}
			}
		}
		int ans = INT_MAX;
		for(int l = 1; l <= num_c; l++) {
			ans = min(ans, dp[l][num_c][num_r]);
		}
		return ans;
	};
	
	cout << work(grid) << "\n";
}

