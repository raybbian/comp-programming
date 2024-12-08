#pragma once
#include <algo/common.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

namespace algo::ds {

/*
 * find_by_order(k) - returns iterator to kth largest element (0-indexed) or an
 * iterator to end if it doesn't exist
 *
 * order_of_key(x) - returns the number of elements in the set strictly smaller
 * than x
 */
template <typename T>
using ordered_set =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, less<T>, __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

} // namespace algo::ds
