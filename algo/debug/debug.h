#pragma once
#include "algo/common.h"

template <typename T>
concept printable = requires(T t) {
    { std::cout << t } -> std::same_as<std::ostream &>;
};
template <typename T>
concept iterable = std::ranges::range<T> && (!printable<T>);

template <typename T = void>
T noop() {
}

template <size_t N>
std::ostream &operator<<(std::ostream &os, const std::bitset<N> &v) {
    os << "<";
    for (size_t i = 0; i < N; i++) {
        os << static_cast<char>('0' + v[i]);
    }
    return os << ">";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p) {
    return os << "(" << p.first << ", " << p.second << ")";
}
template <typename... T>
std::ostream &operator<<(std::ostream &os, const std::tuple<T...> &t) {
    os << "(";
    bool first = true;
    auto print = [&os, &first](auto arg) {
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

template <typename T>
void debug(std::string name, T var) {
    std::cerr << "\x1B[31m" << name << ": " << var << "\x1B[0m" << '\n';
}

// https://www.scs.stanford.edu/~dm/blog/va-opt.html
#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__
#define FOR_EACH(macro, ...)                                                   \
    __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                                        \
    macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define DEBUG(x) debug(#x, x);
