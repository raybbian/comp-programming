//
// Created by rayb on 2/3/24.
//
#include <bits/stdc++.h>;

using namespace std;


int main(){
#define int long long 
    int n; cin >> n;
    string s; cin >> s;

    deque<int> q[3];
    vector<pair<int, int>> ans;

    int a = 0, b = 0, c= 0, bc=0;

    for(int i = 0; i < 2*n ; i++){
        if(s[i] == 'A') a++;
        else if(s[i] == 'B') b++;
        else c++;
    }

    bc = n-a;

    if(bc < 0 ){
        cout << "NO";
        return 0;
    }

    for(int i = 0; i < 2*n ; i++){
        if(s[i] == 'A'){
            q[0].push_back(i);
        }
        else if(s[i] == 'B'){
            if(bc){
                q[1].push_back(i);
                bc--;
            }
            else{
                if(q[0].size() == 0){
                    cout << "NO";
                    return 0;
                }
                ans.push_back(make_pair(q[0].back(), i));
                q[0].pop_back();
            }
        }
        else{
            if(q[0].size() == 0 && q[1].size() == 0){
                cout << "NO";
                return 0;
            }
            if(q[1].size()){
                ans.push_back(make_pair(q[1].back(), i));
                q[1].pop_back();
            }
            else if(q[0].size()){
                ans.push_back(make_pair(q[0].back(), i));
                q[0].pop_back();
            }
        }
    }
    cout << "YES\n";
    for(auto x : ans){
        cout << x.first + 1 << " " << x.second + 1 << "\n";
    }
}