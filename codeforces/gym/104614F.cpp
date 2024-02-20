#include <bits/stdc++.h>
using namespace std;

double Pi = acos(-1);

int main(){
    int r, s, h; cin >> r >> s >> h;
    double days = 2*Pi*r/s/h;
    double diff = abs(days - floor(days + 0.5));
    int n1 = 0, n2 = 0, n3 = 0;
    double res = numeric_limits<float>::max();
    for(int x = 1; x <= 1000; x++){
        for(int y = 2*x; y <= 1000; y += x){
            for(int k = 2*y; k <= 1000; k += y){

                double v = abs(diff- 1.0/x + 1.0/y - 1.0/k);

//                cout << v << endl;
                if (v < res){
                    res = v;
                    n1 = x, n2 = y, n3 = k;
                }
            }
        }
    }
    cout << n1 << "  " << n2 << ' ' << n3;
}