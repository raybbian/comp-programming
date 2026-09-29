#pragma once
#include "algo/common.h"
#include "algo/debug/preamble.h"

template <size_t N>
std::ostream &operator<<(std::ostream &os, const std::bitset<N> &v) {
    os << "<";
    for (size_t i = 0; i < N; i++) {
        os << static_cast<char>('0' + v[i]);
    }
    return os << ">";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, std::queue<T, U> q) {
    os << "[";
    bool first = true;
    for (; !q.empty(); q.pop()) {
        if (!first) os << ", ";
        first = false;
        os << q.front();
    }
    return os << "]";
}
template <typename T, typename U, typename V>
std::ostream &operator<<(std::ostream &os, std::priority_queue<T, U, V> pq) {
    os << "[";
    bool first = true;
    for (; !pq.empty(); pq.pop()) {
        if (!first) os << ", ";
        first = false;
        os << pq.top();
    }
    return os << "]";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}
template <typename... T>
std::ostream &operator<<(std::ostream &os, const std::tuple<T...> &t) {
    os << "(";
    bool first = true;
    auto print = [&os, &first](const auto &arg) {
        if (!first) os << ", ";
        first = false;
        os << arg;
    };
    std::apply([&print](auto &&...args) { (print(args), ...); }, t);
    return os << ")";
}
template <iterable T>
std::ostream &operator<<(std::ostream &os, const T &t) {
    os << "[";
    bool first = true;
    for (const auto &e : t) {
        if (!first) os << ", ";
        first = false;
        os << e;
    }
    return os << "]";
}

inline void debug() {
}
// Takes (name, value) pairs. dbg("hi") prints `hi`; dbg(s) prints `s: <value>`.
template <typename T, typename... Rest>
void debug(std::string_view name, const T &var, const Rest &...rest) {
    std::cout << "\x1B[31m";
    if (!name.starts_with('"')) std::cout << name << ": ";
    std::cout << var << "\x1B[0m" << '\n';
    std::cout.flush();
    debug(rest...);
}

// DBG_ARGS(a, b) expands to #a, a, #b, b. Up to 16 arguments.
#define DBG_1(a) #a, a
#define DBG_2(a, ...) #a, a, DBG_1(__VA_ARGS__)
#define DBG_3(a, ...) #a, a, DBG_2(__VA_ARGS__)
#define DBG_4(a, ...) #a, a, DBG_3(__VA_ARGS__)
#define DBG_5(a, ...) #a, a, DBG_4(__VA_ARGS__)
#define DBG_6(a, ...) #a, a, DBG_5(__VA_ARGS__)
#define DBG_7(a, ...) #a, a, DBG_6(__VA_ARGS__)
#define DBG_8(a, ...) #a, a, DBG_7(__VA_ARGS__)
#define DBG_9(a, ...) #a, a, DBG_8(__VA_ARGS__)
#define DBG_10(a, ...) #a, a, DBG_9(__VA_ARGS__)
#define DBG_11(a, ...) #a, a, DBG_10(__VA_ARGS__)
#define DBG_12(a, ...) #a, a, DBG_11(__VA_ARGS__)
#define DBG_13(a, ...) #a, a, DBG_12(__VA_ARGS__)
#define DBG_14(a, ...) #a, a, DBG_13(__VA_ARGS__)
#define DBG_15(a, ...) #a, a, DBG_14(__VA_ARGS__)
#define DBG_16(a, ...) #a, a, DBG_15(__VA_ARGS__)
#define DBG_PICK(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14,  \
                 _15, _16, NAME, ...)                                          \
    NAME
#define DBG_ARGS(...)                                                          \
    DBG_PICK(__VA_ARGS__, DBG_16, DBG_15, DBG_14, DBG_13, DBG_12, DBG_11,      \
             DBG_10, DBG_9, DBG_8, DBG_7, DBG_6, DBG_5, DBG_4, DBG_3, DBG_2,   \
             DBG_1)(__VA_ARGS__)

// dbg(a, b) prints `a: ...` and `b: ...` in red. Does nothing without -DLOCAL.
#ifdef LOCAL
#define dbg(...) debug(DBG_ARGS(__VA_ARGS__))
#else
#define dbg(...) no_debug(__VA_ARGS__)
#endif
