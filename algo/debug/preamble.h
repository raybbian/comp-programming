#pragma once
#include "algo/common.h"

template <typename T>
concept printable = requires(T t) {
    { std::cout << t } -> std::same_as<std::ostream &>;
};
template <typename T>
concept iterable = std::ranges::range<T> && (!printable<T>);

// Swallows dbg(...) arguments when LOCAL is off, without copying them.
template <typename... T>
inline void no_debug(const T &...) {
}

// Declared early so printers can call each other on nested types, e.g.
// vector<pair<int, set<int>>>. Defined in debug.h.
template <size_t N>
std::ostream &operator<<(std::ostream &os, const std::bitset<N> &v);
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, std::queue<T, U> q);
template <typename T, typename U, typename V>
std::ostream &operator<<(std::ostream &os, std::priority_queue<T, U, V> pq);
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p);
template <typename... T>
std::ostream &operator<<(std::ostream &os, const std::tuple<T...> &t);
template <iterable T>
std::ostream &operator<<(std::ostream &os, const T &t);
