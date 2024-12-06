#pragma once
#include <algo/common.h>

constexpr int popcnt(ll x) { return __builtin_popcountll(x); }
constexpr int log2floor(ll x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); } // floor(log_2(x))
