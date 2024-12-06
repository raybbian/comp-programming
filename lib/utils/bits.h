#pragma once

constexpr int popcnt(long long x) { return __builtin_popcountll(x); }
constexpr int bits(long long x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); } // floor(log_2(x))
