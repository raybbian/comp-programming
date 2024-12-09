#line 2 "lib/algo/utils/rng.h"
#include <algo/common.h>

namespace algo::random {

uint64_t rng() {
    static std::mt19937_64 rng(
        std::chrono::steady_clock::now().time_since_epoch().count());
    return rng();
}

} // namespace algo::random
