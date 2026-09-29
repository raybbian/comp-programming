#pragma once
#include "algo/common.h"

#ifndef PREPROCESS
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#endif

namespace algo::ds {

// std::set that can also look up by position.
//   ordered_set<int> s;
//   s.insert(x);
//   *s.find_by_order(k);  // k-th smallest, 0-indexed (end() if none)
//   s.order_of_key(x);    // number of elements < x
template <typename T>
using ordered_set =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, std::less<T>,
                     __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

} // namespace algo::ds
