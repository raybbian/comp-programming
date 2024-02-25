#include <bits/stdc++.h>
using namespace std;

#define int long long

signed main() {
	int n, m, s, p, q;
	cin >> n >> m >> s >> p >> q;
	int pages = (n + m - 1) / m;

	vector<set<int>> selected(pages + 1);
	vector<set<int>> want(pages + 1);
	for(int i = 0; i < p; i++) {
		int k;
		cin >> k;
		int page = (k - 1) / m + 1;
		selected[page].insert(k);
	}
	for(int i = 0; i < q; i++) {
		int k;
		cin >> k;
		int page = (k - 1) / m + 1;
		want[page].insert(k);
	}

	vector<int> ans(pages + 1);
	int out = 0;
	int first_page = -1, last_page = -1;
	bool found = false;
	for(int i = 1; i <= pages; i++) {
		int on_page;
		if (i == pages) on_page = (n % m == 0 ? m : n % m);
		else on_page = m;
		int select_all_deselect = (selected[i].size() != on_page) + (on_page - (int)want[i].size());
		int deselect_all_select = (selected[i].size() != 0) + (int)want[i].size();
		int manual = 0;
		for(auto &j: want[i]) {
			if (!selected[i].count(j)) {
				manual++;
			}
		}
		for(auto &j: selected[i]) {
			if (!want[i].count(j)) {
				manual++;
			}
		}
		ans[i] = min({select_all_deselect, deselect_all_select, manual});
		if (ans[i] != 0 && !found) {
			first_page = i;
			found = true;
		}
		if (ans[i] != 0) {
			last_page = i;
		}
		out += ans[i];
	}

	if (first_page == -1 && last_page == -1) {
		cout << 0 << "\n";
		return 0;
	}

	if (first_page <= s && s <= last_page) {
		out += last_page - first_page + min(s - first_page, last_page - s);
	} else if (first_page <= s && last_page <= s) {
		out += s - first_page;
	} else if (s <= first_page && s <= last_page) {
		out += last_page - s;
	} else {
		assert(69 == 420);
	}

	cout << out << "\n";
}
