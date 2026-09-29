#pragma once
#include "algo/common.h"
#include "algo/other/custom_hash.h"

namespace algo::utils {

// Memoized recursive function, like Python's @cache. The lambda gets itself as
// its first argument.
//   auto fib = cache<int64_t(int)>([&](auto &fib, int n) -> int64_t {
//       return n < 2 ? n : fib(n - 1) + fib(n - 2);
//   });
template <typename Sig, typename F>
struct cached;
template <typename R, typename... Args, typename F>
struct cached<R(Args...), F> {
    F f;
    std::unordered_map<std::tuple<Args...>, R, hash::chash> memo;
    R operator()(Args... args) {
        std::tuple key(args...);
        if (auto it = memo.find(key); it != memo.end()) return it->second;
        // Recursion may rehash memo, so insert only after it returns.
        R r = f(*this, args...);
        return memo[key] = r;
    }
};
template <typename Sig, typename F>
    requires std::is_function_v<Sig>
auto cache(F f) {
    return cached<Sig, F>{f, {}};
}

} // namespace algo::utils
