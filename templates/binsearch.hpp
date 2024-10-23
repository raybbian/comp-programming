template <typename T, typename U> T last_true(T l, T r, U f) {
    l--;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l + 1) / 2;
        f(mid) ? l = mid : r = mid - 1;
    }
    return l;
}
template <typename T, typename U> T first_true(T l, T r, U f) {
    r++;
    assert(l <= r);
    while (l < r) {
        T mid = l + (r - l) / 2;
        f(mid) ? r = mid : l = mid + 1;
    }
    return l;
}
