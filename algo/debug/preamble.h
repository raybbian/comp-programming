#pragma once
#include "algo/common.h"

template <typename T>
concept string_like = std::convertible_to<const T &, std::string_view>;
template <typename T>
concept iterable = std::ranges::range<T> && (!string_like<T>);
template <typename T>
concept iterator_like = std::input_iterator<T> && (!std::is_pointer_v<T>) &&
                        (!std::ranges::range<T>);

template <typename... T>
inline void no_debug(const T &...) {
}

template <size_t N>
std::ostream &operator<<(std::ostream &os, const std::bitset<N> &v);
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, std::queue<T, U> q);
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, std::stack<T, U> s);
template <typename T, typename U, typename V>
std::ostream &operator<<(std::ostream &os, std::priority_queue<T, U, V> pq);
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p);
template <typename... T>
std::ostream &operator<<(std::ostream &os, const std::tuple<T...> &t);
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::optional<T> &o);
std::ostream &operator<<(std::ostream &os, unsigned __int128 v);
std::ostream &operator<<(std::ostream &os, __int128 v);
template <iterable T>
std::ostream &operator<<(std::ostream &os, const T &t);
template <iterator_like T>
std::ostream &operator<<(std::ostream &os, const T &it);
