#include <bits/stdc++.h>
using namespace std;

template <typename A, typename B> string to_string(const pair<A, B> &t);
template <typename A, typename B, typename C> string to_string(const tuple<A, B, C> &t);
template <typename A, typename B, typename C, typename D>
string to_string(const tuple<A, B, C, D> &t);

string to_string(char c) { return string(1, c); }
string to_string(const string &s) { return '"' + s + '"'; }
string to_string(const char *s) { return to_string((string)s); }
string to_string(bool b) { return (b ? "T" : "F"); }
string to_string(vector<bool> v) {
    int n = (int)v.size();
    string res = "[";
    for (int i = 0; i < n; i++) {
        if (i)
            res += ", ";
        res += to_string(v[i]);
    }
    res += "]";
    return res;
}
template <typename A> string to_string(A v) {
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
template <typename A, typename B> string to_string(const pair<A, B> &t) {
    return "(" + to_string(t.first) + ": " + to_string(t.second) + ")";
}
template <typename A, typename B, typename C> string to_string(const tuple<A, B, C> &t) {
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " + to_string(get<2>(t)) +
           ")";
}
template <typename A, typename B, typename C, typename D>
string to_string(const tuple<A, B, C, D> &t) {
    return "(" + to_string(get<0>(t)) + ", " + to_string(get<1>(t)) + ", " + to_string(get<2>(t)) +
           ", " + to_string(get<3>(t)) + ")";
}

void debug() { cerr << endl; }
template <typename Head, typename... Tail> void debug(Head H, Tail... T) {
    cerr << " " << to_string(H);
    debug(T...);
}
