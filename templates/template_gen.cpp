#include "algo/common.h"
#include "algo/gen/gen.h"

using namespace std;
using namespace algo;

signed main(int argc, char **argv) {
    gen::init(argc, argv);
    int n = gen::num(1, 6);
    cout << n << '\n';
    for (int x : gen::vec(n, 1, 10))
        cout << x << ' ';
    cout << '\n';
}
