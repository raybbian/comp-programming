#pragma once
#include <algo/common.h>

namespace algo::search {

// Finds argmax on [l, r]. Function must be strictly concave!
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

// Finds argmin on [l, r]. Function must be strictly convex!
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

// Returns l-1 if no values are true in range.
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

// Returns r+1 if no values are true in range.
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
