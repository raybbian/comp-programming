#pragma once
#include "algo/common.h"
#include "algo/math/common.h"
#include "algo/math/primality.h"

namespace algo::math {

template <int id>
struct modint {
    static int mod() {
        return bt.mod();
    }
    static void set_mod(int m) {
        assert(1 <= m);
        bt = barrett(m);
    }
    auto static with_mod(int tmp, auto callback) {
        struct scoped {
            int prev = mod();
            ~scoped() {
                set_mod(prev);
            }
        } _;
        set_mod(tmp);
        return callback();
    }
    modint() : v(0) {
    }
    modint(int64_t _v) {
        v = (-mod() < _v && _v < mod()) ? _v : _v % mod();
        if (v < 0) v += mod();
    }
    modint &operator+=(const modint &other) {
        v += other.v;
        if (v >= mod()) v -= mod();
        return *this;
    }
    modint &operator-=(const modint &other) {
        v -= other.v;
        if (v < 0) v += mod();
        return *this;
    }
    modint &operator*=(const modint &other) {
        v = bt.reduce((int64_t)v * other.v);
        return *this;
    }
    modint &operator/=(const modint &other) {
        return *this = *this * other.inv();
    }
    int &operator++() {
        v++;
        if (v == mod()) v = 0;
        return *this;
    }
    modint &operator--() {
        if (v == 0) v = mod();
        v--;
        return *this;
    }
    modint operator++(int) {
        modint result = *this;
        ++*this;
        return result;
    }
    modint operator--(int) {
        modint result = *this;
        --*this;
        return result;
    }
    friend modint operator+(modint a, const modint &b) {
        return a += b;
    }
    friend modint operator-(modint a, const modint &b) {
        return a -= b;
    }
    friend modint operator*(modint a, const modint &b) {
        return a *= b;
    }
    friend modint operator/(modint a, const modint &b) {
        return a /= b;
    }
    friend modint operator-(modint a) {
        return 0 - a;
    }
    modint inv() const {
        auto eg = inv_gcd(v, mod());
        assert(eg.first == 1);
        return eg.second;
    }
    friend bool operator==(const modint &a, const modint &b) {
        return a.v == b.v;
    }
    friend bool operator!=(const modint &a, const modint &b) {
        return !(a == b);
    }
    explicit operator int() const {
        return v;
    }
    friend std::ostream &operator<<(std::ostream &os, const modint &a) {
        return os << a.v;
    }
    friend std::istream &operator>>(std::istream &is, modint &a) {
        is >> a.v;
        a.v = (mod() < a.v && a.v < mod()) ? a.v : a.v % mod();
        if (a.v < 0) a.v += mod();
        return is;
    }

private:
    int v;
    static barrett bt;
};
template <int id>
barrett modint<id>::bt(998244353);

using mint = modint<-1>;

} // namespace algo::math
