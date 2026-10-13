#pragma once
#include "algo/common.h"
#include "algo/gen/random.h"

namespace algo::gen {

// n values in [lo, hi]; bias as in num.
//   vec(5, 1, 3);  // [2, 1, 3, 3, 1]
template <std::integral T, std::integral U>
std::vector<std::common_type_t<T, U>> vec(int n, T lo, U hi, int bias = 0) {
    std::vector<std::common_type_t<T, U>> v(n);
    for (auto &x : v)
        x = num(lo, hi, bias);
    return v;
}

// n distinct values in [lo, hi].
//   distinct(4, 1, 10);  // [7, 2, 10, 4]
template <std::integral T, std::integral U>
std::vector<std::common_type_t<T, U>> distinct(int n, T lo, U hi) {
    using R = std::common_type_t<T, U>;
    uint64_t span = uint64_t(hi) - uint64_t(lo) + 1;
    assert(n >= 0 && uint64_t(n) <= span);
    std::vector<R> v;
    if (span < 2 * uint64_t(n)) {
        v.resize(span);
        std::iota(v.begin(), v.end(), R(lo));
        shuffle(v);
        v.resize(n);
    } else {
        std::set<R> seen;
        while ((int)v.size() < n) {
            R x = num(lo, hi);
            if (seen.insert(x).second) v.push_back(x);
        }
    }
    return v;
}

// n values in [lo, hi], non-decreasing (or strictly increasing).
//   sorted(5, 1, 9);        // [1, 3, 3, 8, 9]
//   sorted(5, 1, 9, true);  // [1, 3, 4, 8, 9]
template <std::integral T, std::integral U>
std::vector<std::common_type_t<T, U>> sorted(int n, T lo, U hi,
                                             bool strict = false) {
    auto v = strict ? distinct(n, lo, hi) : vec(n, lo, hi);
    std::sort(v.begin(), v.end());
    return v;
}

// k parts, each >= min_part, summing to n.
//   split(10, 3);     // [4, 1, 5]
//   split(10, 3, 0);  // [0, 7, 3]
template <std::integral T, std::integral U = T>
std::vector<T> split(T n, int k, U min_part = 1) {
    T rest = n - T(min_part) * k;
    assert(k >= 1 && rest >= 0);
    auto cuts = sorted(k - 1, T(0), rest);
    cuts.insert(cuts.begin(), 0);
    cuts.push_back(rest);
    std::vector<T> parts(k);
    for (int i = 0; i < k; i++) {
        parts[i] = cuts[i + 1] - cuts[i] + T(min_part);
    }
    return parts;
}

// String of n characters from alphabet, or from the range [lo, hi].
//   str(6, "ab");      // "abbaba"
//   str(6, 'a', 'c');  // "cabacc"
inline std::string str(int n, std::string_view alphabet) {
    std::string s(n, 0);
    for (auto &c : s)
        c = pick(alphabet);
    return s;
}
inline std::string str(int n, char lo, char hi) {
    std::string s(n, 0);
    for (auto &c : s)
        c = char(num(lo, hi));
    return s;
}

// Palindrome of length n over alphabet.
//   palindrome(5, "ab");  // "abbba"
inline std::string palindrome(int n, std::string_view alphabet) {
    std::string s = str((n + 1) / 2, alphabet);
    s.resize(n);
    for (int i = 0; i < n / 2; i++)
        s[n - 1 - i] = s[i];
    return s;
}

// Balanced sequence of n bracket pairs.
//   brackets(3);        // "(()())"
//   brackets(3, "[]");  // "[[]][]"
inline std::string brackets(int n, std::string_view chars = "()") {
    std::string s;
    int open = 0, close = 0;
    while (close < n) {
        if (open < n && (open == close || chance(0.5))) {
            s += chars[0], open++;
        } else {
            s += chars[1], close++;
        }
    }
    return s;
}

// n x m grid of characters from alphabet.
//   grid(2, 3, ".#");  // [".#.", "##."]
inline std::vector<std::string> grid(int n, int m, std::string_view alphabet) {
    std::vector<std::string> g(n);
    for (auto &row : g)
        row = str(m, alphabet);
    return g;
}

// n x m matrix of values in [lo, hi].
//   matrix(2, 3, 0, 9);  // [[3, 0, 7], [9, 9, 1]]
template <std::integral T, std::integral U>
std::vector<std::vector<std::common_type_t<T, U>>> matrix(int n, int m, T lo,
                                                          U hi) {
    std::vector<std::vector<std::common_type_t<T, U>>> a(n);
    for (auto &row : a)
        row = vec(m, lo, hi);
    return a;
}

// k intervals [l, r] with lo <= l <= r <= hi.
//   intervals(3, 1, 10);  // [(2, 7), (5, 5), (1, 10)]
template <std::integral T, std::integral U>
auto intervals(int k, T lo, U hi) {
    using R = std::common_type_t<T, U>;
    std::vector<std::pair<R, R>> v(k);
    for (auto &[l, r] : v) {
        l = num(lo, hi), r = num(lo, hi);
        if (l > r) std::swap(l, r);
    }
    return v;
}

} // namespace algo::gen
