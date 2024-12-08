#pragma once
#include <algo/common.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"

template <typename A, typename B>
string to_string(const pair<A, B> &t);
template <typename A, typename B, typename C>
string to_string(const tuple<A, B, C> &t);
template <typename A, typename B, typename C, typename D>
string to_string(const tuple<A, B, C, D> &t);

string to_string(char c) {
    return string(1, c);
}
string to_string(const string &s) {
    return '"' + s + '"';
}
string to_string(const char *s) {
    return to_string((string)s);
}
string to_string(bool b) {
    return (b ? "T" : "F");
}
string to_string(vector<bool> v) {
    string res = "[";
    for (int i = 0; i < sz(v); i++) {
        if (i) res += ", ";
        res += to_string(v[i]);
    }
    res += "]";
    return res;
}
template <size_t N>
string to_string(bitset<N> v) {
    string res = "<";
    for (size_t i = 0; i < N; i++) {
        res += static_cast<char>('0' + v[i]);
    }
    res += ">";
    return res;
}
template <typename A>
string to_string(A v) {
    bool first = true;
    string res = "[";
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
template <typename A, typename B>
string to_string(const pair<A, B> &t) {
    return "(" + to_string(t.fi) + ": " + to_string(t.se) + ")";
}
template <typename A, typename B, typename C>
string to_string(const tuple<A, B, C> &t) {
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " +
           to_string(get<2>(t)) + ")";
}
template <typename A, typename B, typename C, typename D>
string to_string(const tuple<A, B, C, D> &t) {
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " +
           to_string(get<2>(t)) + ", " + to_string(get<3>(t)) + ")";
}

template <typename T>
void debug(string name, T var) {
    cerr << RED << name << ": " << to_string(var) << RESET << '\n';
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
