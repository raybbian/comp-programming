#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// Factorials and binomials mod p. Tables grow as needed.
//   combo<mint> c;
//   c.cmb(n, r);   // n choose r
//   c.perm(n, r);  // n! / (n - r)!
template <typename T>
struct combo {
    explicit combo(int n = 0) {
        if (n > 0) fact(n), inv_fact(n);
    }

    T fact(int n) {
        if (n >= (int)f.size()) {
            assert(n < mod());
            if (f.empty()) f.push_back(T(1));
            int m = grow_to(n, (int)f.size());
            f.reserve(m);
            for (int i = (int)f.size(); i < m; i++) {
                f.push_back(f.back() * T(i));
            }
        }
        return f[n];
    }
    T inv_fact(int n) {
        if (n >= (int)inv_f.size()) {
            assert(n < mod());
            if (inv_f.empty()) inv_f.push_back(T(1));
            int lo = (int)inv_f.size(), m = grow_to(n, lo);
            inv_f.resize(m);
            inv_f[m - 1] = T(1) / fact(m - 1);
            for (int i = m - 2; i >= lo; i--) {
                inv_f[i] = inv_f[i + 1] * T(i + 1);
            }
        }
        return inv_f[n];
    }
    T cmb(int n, int r) {
        if (r < 0 || r > n) {
            return T(0);
        } else {
            return fact(n) * inv_fact(r) * inv_fact(n - r);
        }
    }
    T perm(int n, int r) {
        if (r < 0 || r > n) {
            return T(0);
        } else {
            return fact(n) * inv_fact(n - r);
        }
    }

private:
    std::vector<T> f, inv_f;

    static int mod() {
        if constexpr (requires { T::mod(); }) {
            return T::mod();
        } else {
            return std::numeric_limits<int>::max();
        }
    }
    // New table size. Capped at mod, since n! = 0 for n >= mod.
    static int grow_to(int n, int cur) {
        return std::min<int64_t>(std::max<int64_t>(n + 1, 2LL * cur), mod());
    }
};

} // namespace algo::math
