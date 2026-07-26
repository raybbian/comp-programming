#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// Factorial tables sized on demand by doubling. State lives here rather than in
// statics, so two moduli are two objects and neither can go stale.
template <typename T>
struct combo {
    explicit combo(index_t n = 0) {
        if (n > 0) fact(n), inv_fact(n);
    }

    T fact(index_t n) {
        if (n >= (index_t)f.size()) {
            if (n >= mod()) return T(0);
            if (f.empty()) f.push_back(T(1));
            index_t m = grow_to(n, (index_t)f.size());
            f.reserve(m);
            for (index_t i = (index_t)f.size(); i < m; i++) {
                f.push_back(f.back() * T(i));
            }
        }
        return f[n];
    }
    T inv_fact(index_t n) {
        if (n >= (index_t)inv_f.size()) {
            if (n >= mod()) return T(0);
            if (inv_f.empty()) inv_f.push_back(T(1));
            index_t lo = (index_t)inv_f.size(), m = grow_to(n, lo);
            inv_f.resize(m);
            inv_f[m - 1] = T(1) / fact(m - 1);
            for (index_t i = m - 2; i >= lo; i--) {
                inv_f[i] = inv_f[i + 1] * T(i + 1);
            }
        }
        return inv_f[n];
    }
    T cmb(index_t n, index_t r) {
        if (r < 0 || r > n) {
            return T(0);
        } else {
            return fact(n) * inv_fact(r) * inv_fact(n - r);
        }
    }
    T perm(index_t n, index_t r) {
        if (r < 0 || r > n) {
            return T(0);
        } else {
            return fact(n) * inv_fact(n - r);
        }
    }

private:
    std::vector<T> f, inv_f;

    static index_t mod() {
        if constexpr (requires { T::mod(); }) {
            return T::mod();
        } else {
            return std::numeric_limits<index_t>::max();
        }
    }
    // n! is 0 once n >= mod (mod divides it) and has no inverse, so neither
    // table grows past the modulus.
    static index_t grow_to(index_t n, index_t cur) {
        return std::min<int64_t>(std::max<int64_t>(n + 1, 2LL * cur), mod());
    }
};

} // namespace algo::math
