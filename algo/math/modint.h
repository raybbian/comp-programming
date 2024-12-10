#pragma once
#include "../common.h"
#include "../math/common.h"
#include "../math/primality.h"

/*
 * Copied from
 * https://github.com/cp-algorithms/cp-algorithms-aux/blob/main/cp-algo/number_theory/mint.hpp
 */

namespace algo::math {

template <int m>
struct static_modint {
    using mint = static_modint;
    static constexpr int mod() {
        return m;
    }
    static_modint() : v(0) {
    }
    static_modint(int64_t _v) {
        v = (-m < _v && _v < m) ? _v : _v % m;
        if (v < 0) v += m;
    }
    mint &operator+=(const mint &other) {
        v += other.v;
        if (v >= m) v -= m;
        return *this;
    }
    mint &operator-=(const mint &other) {
        v -= other.v;
        if (v < 0) v += m;
        return *this;
    }
    mint &operator*=(const mint &other) {
        v = v * other.v % m;
        return *this;
    }
    mint &operator/=(const mint &other) {
        return *this *= bpow(other, m - 2);
    }
    mint &operator++() {
        v++;
        if (v == m) v = 0;
        return *this;
    }
    mint &operator--() {
        if (v == 0) v = m;
        v--;
        return *this;
    }
    mint operator++(int) {
        mint result = *this;
        ++*this;
        return result;
    }
    mint operator--(int) {
        mint result = *this;
        --*this;
        return result;
    }
    friend mint operator+(mint a, const mint &b) {
        return a += b;
    }
    friend mint operator-(mint a, const mint &b) {
        return a -= b;
    }
    friend mint operator*(mint a, const mint &b) {
        return a *= b;
    }
    friend mint operator/(mint a, const mint &b) {
        return a /= b;
    }
    friend mint operator-(mint a) {
        return 0 - a;
    }
    mint inv() const {
        if (prime) {
            assert(v);
            return bpow(v, m - 2);
        } else {
            auto eg = inv_gcd(v, m);
            assert(eg.first == 1);
            return eg.second;
        }
    }

    friend bool operator==(const mint &a, const mint &b) {
        return a.v == b.v;
    }
    friend bool operator!=(const mint &a, const mint &b) {
        return !(a == b);
    }
    explicit operator int() const {
        return v;
    }
    friend std::ostream &operator<<(std::ostream &os, const mint &a) {
        return os << a.v;
    }
    friend std::istream &operator>>(std::istream &is, mint &a) {
        is >> a.v;
        a.v = (-m < a.v && a.v < m) ? a.v : a.v % m;
        if (a.v < 0) a.v += m;
        return is;
    }

private:
    int v;
    static constexpr bool prime = is_prime<m>;
};

template <int id>
struct dynamic_modint {
    using mint = dynamic_modint;

    static int mod() {
        return (int)(bt.umod());
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
    dynamic_modint() : v(0) {
    }
    dynamic_modint(int64_t _v) {
        v = (-mod() < _v && _v < mod()) ? _v : _v % mod();
        if (v < 0) v += mod();
    }
    mint &operator+=(const mint &other) {
        v += other.v;
        if (v >= mod()) v -= mod();
        return *this;
    }
    mint &operator-=(const mint &other) {
        v -= other.v;
        if (v < 0) v += mod();
        return *this;
    }
    mint &operator*=(const mint &other) {
        v = bt.mul(v, other.v);
        return *this;
    }
    mint &operator/=(const mint &other) {
        return *this = *this * other.inv();
    }
    int &operator++() {
        v++;
        if (v == mod()) v = 0;
        return *this;
    }
    mint &operator--() {
        if (v == 0) v = mod();
        v--;
        return *this;
    }
    mint operator++(int) {
        mint result = *this;
        ++*this;
        return result;
    }
    mint operator--(int) {
        mint result = *this;
        --*this;
        return result;
    }
    friend mint operator+(mint a, const mint &b) {
        return a += b;
    }
    friend mint operator-(mint a, const mint &b) {
        return a -= b;
    }
    friend mint operator*(mint a, const mint &b) {
        return a *= b;
    }
    friend mint operator/(mint a, const mint &b) {
        return a /= b;
    }
    friend mint operator-(mint a) {
        return 0 - a;
    }

    mint inv() const {
        auto eg = inv_gcd(v, mod());
        assert(eg.first == 1);
        return eg.second;
    }

    friend bool operator==(const mint &a, const mint &b) {
        return a.v == b.v;
    }
    friend bool operator!=(const mint &a, const mint &b) {
        return !(a == b);
    }
    explicit operator int() const {
        return v;
    }
    friend std::ostream &operator<<(std::ostream &os, const mint &a) {
        return os << a.v;
    }
    friend std::istream &operator>>(std::istream &is, mint &a) {
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
barrett dynamic_modint<id>::bt(998244353);

using mint998 = static_modint<998244353>;
using mint107 = static_modint<1000000007>;
using mint = dynamic_modint<-1>;

} // namespace algo::math
