#include <bits/stdc++.h>

using namespace std;

#define int long long
vector<int> adj[5002];
string b;
int broke_with[5002];

char match(char c){
    if(c == '}') return '{';
    else if(c == ']') return '[';
    else if(c == ')') return '(';
    return '.';
}

int dfs(int x, vector<int> &q, int p){
    int ans = 0;
    if(q.size() && b[q.back()-1] == match(b[x-1])){
		broke_with[x] = q.back();
        q.pop_back();
        ans += (int)q.empty();
    }
    else{
        q.push_back(x);
    }

    for(int i : adj[x]){
        if(i == p) continue;
        ans += dfs(i, q, x);
    }
	if (!q.size() || q.back() != x) {
		assert(match(b[x-1]) != '.');
		assert(broke_with[x] != 0);
		q.push_back(broke_with[x]);
	} else {
		q.pop_back();
	}

    return ans;
}

signed main() {
    int n; cin >> n;
    cin >> b;
    for (int i = 0; i < n-1; i++){
        int x, y; cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    int ans = 0;
    for(int i = 1; i<= n; i++){
        vector<int> q;
//        q.push(b[i - 1]);
        int tmp = dfs(i, q, -1);
//        cout << i << " " << tmp << endl;
        ans += tmp;
//        ans += dfs(i, q, -1);
    }
    cout << ans;
}

