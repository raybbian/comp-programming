#pragma once
#include "algo/common.h"

namespace algo::search {

// x in [l, r] maximizing f. f must be strictly concave.
template <typename U>
double argmax(double l, double r, U f, double eps = 1e-9) {
    while (r - l > eps) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;
        if (f(m1) < f(m2))
            l = m1;
        else
            r = m2;
    }
    return l;
}

// x in [l, r] minimizing f. f must be strictly convex.
template <typename U>
double argmin(double l, double r, U f, double eps = 1e-9) {
    while (r - l > eps) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;
        if (f(m1) > f(m2))
            l = m1;
        else
            r = m2;
    }
    return l;
}

// Last x in [l, r] with f(x) true, for f true...true false...false.
// Returns l - 1 if none.
//   last_true(0, n, [&](int x) { return x * x <= n; });  // floor(sqrt(n))
template <typename T, typename U>
T last_true(T l, T r, U f) {
    l--;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l + 1) / 2;
        f(mid) ? l = mid : r = mid - 1;
    }
    return l;
}

// First x in [l, r] with f(x) true, for f false...false true...true.
// Returns r + 1 if none.
//   first_true(0, n, [&](int i) { return a[i] >= x; });  // lower_bound
template <typename T, typename U>
T first_true(T l, T r, U f) {
    r++;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l) / 2;
        f(mid) ? r = mid : l = mid + 1;
    }
    return l;
}

} // namespace algo::search
