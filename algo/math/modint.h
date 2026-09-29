#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// Modulus fixed at compile time.
template <int Mod>
struct static_mod {
    static constexpr int mod() {
        return Mod;
    }
    static int reduce(uint64_t x) {
        return (int)(x % (uint64_t)Mod);
    }
};

// Modulus set at run time, valid only inside with_mod. Calls can't nest; use
// another id for a second modulus.
//   using mint = dynamic_modint<>;
//   mint::with_mod(m, [&] { ... });
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

// Integer mod P::mod(). P provides mod() and reduce(x) = x % mod.
//   using mint = static_modint<998244353>;
//   mint a = 5;
//   a /= 3;
//   int(a);
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
