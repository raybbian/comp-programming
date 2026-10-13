#pragma once
#include "algo/common.h"

namespace algo::brute {

// Range over first, then advance(value) until it returns false.
//   for (auto &x : enumerate(0, [](int &x) { return ++x < 3; }))  // 0, 1, 2
template <typename T, typename F>
struct enumeration {
    T value;
    F advance;
    bool done;
    struct iterator {
        enumeration *e;
        const T &operator*() const {
            return e->value;
        }
        iterator &operator++() {
            e->done = !e->advance(e->value);
            return *this;
        }
        bool operator==(std::default_sentinel_t) const {
            return e->done;
        }
    };
    iterator begin() {
        return {this};
    }
    std::default_sentinel_t end() {
        return {};
    }
};
template <typename T, typename F>
enumeration<T, F> enumerate(T first, F advance, bool empty = false) {
    return {std::move(first), std::move(advance), empty};
}

// Same enumeration with f applied to every value.
template <typename T, typename F, typename G>
auto mapped(enumeration<T, F> e, G f) {
    auto first = f(e.value);
    return enumerate(
        std::move(first),
        [e = std::move(e), f](auto &out) mutable {
            if (!e.advance(e.value)) return false;
            out = f(e.value);
            return true;
        },
        e.done);
}

// Increment v as a base-(hi - lo + 1) number with digits in [lo, hi].
inline bool bump(std::vector<int> &v, int lo, int hi) {
    for (int i = (int)v.size() - 1; i >= 0; i--) {
        if (v[i] < hi) {
            v[i]++;
            return true;
        }
        v[i] = lo;
    }
    return false;
}

// Length-n vectors with values in [lo, hi], or in [0, m). Lexicographic.
//   sequences(2, 2);     // [0, 0], [0, 1], [1, 0], [1, 1]
//   sequences(2, 1, 2);  // [1, 1], [1, 2], [2, 1], [2, 2]
inline auto sequences(int n, int lo, int hi) {
    return enumerate(
        std::vector<int>(n, lo),
        [lo, hi](std::vector<int> &v) { return bump(v, lo, hi); },
        n > 0 && lo > hi);
}
inline auto sequences(int n, int m) {
    return sequences(n, 0, m - 1);
}

// Length-n strings over alphabet. Lexicographic in alphabet order.
//   strings(2, "ab");  // "aa", "ab", "ba", "bb"
inline auto strings(int n, std::string alphabet) {
    return mapped(sequences(n, alphabet.size()), [alphabet](const auto &idx) {
        std::string s(idx.size(), 0);
        for (size_t i = 0; i < idx.size(); i++)
            s[i] = alphabet[idx[i]];
        return s;
    });
}

// Permutations of 0..n-1, or of v's elements (each arrangement once).
//   permutations(3);  // [0, 1, 2], [0, 2, 1], [1, 0, 2], [1, 2, 0], ...
template <typename T>
auto permutations(std::vector<T> v) {
    std::sort(v.begin(), v.end());
    return enumerate(std::move(v), [](std::vector<T> &p) {
        return std::next_permutation(p.begin(), p.end());
    });
}
inline auto permutations(int n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    return permutations(std::move(v));
}

// Subsets of 0..n-1, or of v's elements, in increasing bitmask order.
//   subsets(2);  // [], [0], [1], [0, 1]
template <typename T>
auto subsets(std::vector<T> v) {
    return enumerate(std::vector<T>{},
                     [v, mask = int64_t(0)](std::vector<T> &s) mutable {
                         if (++mask >= int64_t(1) << v.size()) return false;
                         s.clear();
                         for (size_t i = 0; i < v.size(); i++) {
                             if (mask >> i & 1) s.push_back(v[i]);
                         }
                         return true;
                     });
}
inline auto subsets(int n) {
    std::vector<int> v(n);
    std::iota(v.begin(), v.end(), 0);
    return subsets(std::move(v));
}

// Submasks of mask, decreasing, including mask itself and 0.
//   submasks(0b101);  // 5, 4, 1, 0
inline auto submasks(int64_t mask) {
    return enumerate(mask, [mask](int64_t &s) {
        if (s == 0) return false;
        s = (s - 1) & mask;
        return true;
    });
}

// k-element subsets of 0..n-1, or of v's elements, as sorted index vectors.
//   combinations(4, 2);  // [0, 1], [0, 2], [0, 3], [1, 2], [1, 3], [2, 3]
inline auto combinations(int n, int k) {
    std::vector<int> c(std::max(k, 0));
    std::iota(c.begin(), c.end(), 0);
    return enumerate(
        std::move(c),
        [n, k](std::vector<int> &c) {
            int i = k - 1;
            while (i >= 0 && c[i] == n - k + i)
                i--;
            if (i < 0) return false;
            c[i]++;
            for (int j = i + 1; j < k; j++)
                c[j] = c[j - 1] + 1;
            return true;
        },
        k < 0 || k > n);
}
template <typename T>
auto combinations(std::vector<T> v, int k) {
    return mapped(combinations(v.size(), k), [v](const auto &idx) {
        std::vector<T> s;
        for (int i : idx)
            s.push_back(v[i]);
        return s;
    });
}

// k integers >= lo summing to n. Lexicographic.
//   compositions(3, 2);     // [0, 3], [1, 2], [2, 1], [3, 0]
//   compositions(3, 2, 1);  // [1, 2], [2, 1]
inline auto compositions(int n, int k, int lo = 0) {
    int m = n - lo * k;
    std::vector<int> v(std::max(k, 0), lo);
    if (k > 0) v[k - 1] += m;
    return enumerate(
        std::move(v),
        [k, lo](std::vector<int> &v) {
            int t = k - 1;
            while (t >= 0 && v[t] == lo)
                t--;
            if (t <= 0) return false;
            int rem = v[t] - lo;
            v[t] = lo;
            v[t - 1]++;
            v[k - 1] += rem - 1;
            return true;
        },
        k < 0 || m < 0 || (k == 0 && n != 0));
}

// Partitions of n into non-increasing positive parts, largest first.
//   partitions(4);  // [4], [3, 1], [2, 2], [2, 1, 1], [1, 1, 1, 1]
inline auto partitions(int n) {
    return enumerate(std::vector<int>(n > 0, n), [](std::vector<int> &v) {
        int i = (int)v.size() - 1;
        while (i >= 0 && v[i] == 1)
            i--;
        if (i < 0) return false;
        int rem = (int)v.size() - i;
        v[i]--;
        v.resize(i + 1);
        while (rem > 0) {
            v.push_back(std::min(v[i], rem));
            rem -= v.back();
        }
        return true;
    });
}

// Partitions of 0..n-1 into blocks, as block ids (restricted growth strings).
//   set_partitions(3);  // [0, 0, 0], [0, 0, 1], [0, 1, 0], [0, 1, 1], [0, 1,
//   2]
inline auto set_partitions(int n) {
    return enumerate(std::vector<int>(n, 0), [](std::vector<int> &v) {
        for (int i = (int)v.size() - 1; i >= 1; i--) {
            if (v[i] <= *std::max_element(v.begin(), v.begin() + i)) {
                v[i]++;
                std::fill(v.begin() + i + 1, v.end(), 0);
                return true;
            }
        }
        return false;
    });
}

// Balanced sequences of n bracket pairs. Lexicographic with '(' < ')'.
//   brackets(2);  // "(())", "()()"
inline auto brackets(int n) {
    return enumerate(std::string(n, '(') + std::string(n, ')'),
                     [n](std::string &s) {
                         int o = n;
                         for (int i = 2 * n - 1; i >= 0; i--) {
                             o -= s[i] == '(';
                             if (s[i] == '(' && o > i - o) {
                                 s.resize(i);
                                 s += ')';
                                 s += std::string(n - o, '(');
                                 s += std::string(n - (i - o) - 1, ')');
                                 return true;
                             }
                         }
                         return false;
                     });
}

// Every labeled tree on vertices 1..n as an edge list, via Prüfer codes.
//   trees(3);  // [(2, 1), (1, 3)], [(1, 2), (2, 3)], [(1, 3), (2, 3)]
inline auto trees(int n) {
    return mapped(sequences(std::max(n - 2, 0), 1, n), [n](const auto &code) {
        std::vector<std::pair<int, int>> e;
        std::vector<int> deg(n + 1, 1);
        for (int c : code)
            deg[c]++;
        for (int c : code) {
            int leaf = 1;
            while (deg[leaf] != 1)
                leaf++;
            e.emplace_back(leaf, c);
            deg[leaf]--, deg[c]--;
        }
        if (n >= 2) {
            int u = 1;
            while (deg[u] != 1)
                u++;
            int v = u + 1;
            while (deg[v] != 1)
                v++;
            e.emplace_back(u, v);
        }
        return e;
    });
}

// Every simple graph on vertices 1..n as an edge list, by subset of pairs.
//   graphs(3);  // [], [(1, 2)], [(1, 3)], [(1, 2), (1, 3)], [(2, 3)], ...
inline auto graphs(int n) {
    std::vector<std::pair<int, int>> all;
    for (int u = 1; u <= n; u++) {
        for (int v = u + 1; v <= n; v++)
            all.emplace_back(u, v);
    }
    return subsets(std::move(all));
}

} // namespace algo::brute
