#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// A modulus fixed at compile time: no state, and the division folds into a
// multiply-shift.
template <int Mod>
struct static_mod {
    static constexpr int mod() {
        return Mod;
    }
    static int reduce(uint64_t x) {
        return (int)(x % (uint64_t)Mod);
    }
};

// A modulus known only at run time, held for the extent of with_mod. Nesting is
// rejected: values built under the outer modulus would survive into the inner
// one. Use a second id to hold two moduli at once.
template <int id>
struct dynamic_mod {
    static int mod() {
        assert(armed);
        return bt.mod();
    }
    static int reduce(uint64_t x) {
        return (int)bt.reduce(x);
    }
    static auto with_mod(int m, auto callback) {
        assert(1 <= m && !armed);
        struct scoped {
            ~scoped() {
                armed = false;
            }
        } _;
        bt = barrett(m), armed = true;
        return callback();
    }

private:
    static inline barrett bt{1};
    static inline bool armed = false;
};

// P supplies mod() and reduce(). Inheriting it makes both reachable through the
// modint (as is with_mod), and an empty policy costs no space.
template <typename P>
struct modint : P {
    modint() : v(0) {
    }
    modint(int64_t _v) {
        v = (-P::mod() < _v && _v < P::mod()) ? _v : _v % P::mod();
        if (v < 0) v += P::mod();
    }
    modint &operator+=(const modint &other) {
        v += other.v;
        if (v >= P::mod()) v -= P::mod();
        return *this;
    }
    modint &operator-=(const modint &other) {
        v -= other.v;
        if (v < 0) v += P::mod();
        return *this;
    }
    modint &operator*=(const modint &other) {
        v = P::reduce((uint64_t)v * other.v);
        return *this;
    }
    modint &operator/=(const modint &other) {
        return *this = *this * other.inv();
    }
    modint &operator++() {
        v++;
        if (v == P::mod()) v = 0;
        return *this;
    }
    modint &operator--() {
        if (v == 0) v = P::mod();
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
        auto eg = inv_gcd(v, P::mod());
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
        a.v = (-P::mod() < a.v && a.v < P::mod()) ? a.v : a.v % P::mod();
        if (a.v < 0) a.v += P::mod();
        return is;
    }

private:
    int v;
};

template <int Mod>
using static_modint = modint<static_mod<Mod>>;
template <int id = 0>
using dynamic_modint = modint<dynamic_mod<id>>;

} // namespace algo::math
