#pragma once
#include "algo/common.h"

namespace algo::utils {

template <typename T>
std::vector<T> ndvec(size_t size, T initial_value) {
    return std::vector<T>(size, initial_value);
}
template <typename T, typename... U>
auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return std::vector<decltype(inner)>(head, inner);
}

} // namespace algo::utils
