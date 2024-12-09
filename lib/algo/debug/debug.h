#pragma once
#include "algo/common.h"

template <typename A, typename B>
std::string to_string(const std::pair<A, B> &t);
template <typename A, typename B, typename C>
std::string to_string(const std::tuple<A, B, C> &t);
template <typename A, typename B, typename C, typename D>
std::string to_string(const std::tuple<A, B, C, D> &t);
template <std::ranges::range A>
std::string to_string(A v);

void noop() {
}

std::string to_string(char c) {
    return std::string(1, c);
}
std::string to_string(const std::string &s) {
    return '"' + s + '"';
}
std::string to_string(const char *s) {
    return to_string((std::string)s);
}
std::string to_string(bool b) {
    return (b ? "T" : "F");
}
template <size_t N>
std::string to_string(std::bitset<N> v) {
    std::string res = "<";
    for (size_t i = 0; i < N; i++) {
        res += static_cast<char>('0' + v[i]);
    }
    res += ">";
    return res;
}
template <typename A, typename B>
std::string to_string(const std::pair<A, B> &t) {
    return "(" + to_string(t.fi) + ": " + to_string(t.se) + ")";
}
template <typename A, typename B, typename C>
std::string to_string(const std::tuple<A, B, C> &t) {
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " +
           to_string(get<2>(t)) + ")";
}
template <typename A, typename B, typename C, typename D>
std::string to_string(const std::tuple<A, B, C, D> &t) {
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " +
           to_string(get<2>(t)) + ", " + to_string(get<3>(t)) + ")";
}
template <std::ranges::range A>
std::string to_string(A v) {
    bool first = true;
    std::string res = "[";
    for (const auto &x : v) {
        if (!first) {
            res += ", ";
        }
        first = false;
        res += to_string(x);
    }
    res += "]";
    return res;
}

template <typename T>
void debug(std::string name, T var) {
    std::cerr << "\x1B[31m" << name << ": " << to_string(var) << "\x1B[0m"
              << '\n';
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
