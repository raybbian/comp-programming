#pragma once
#include <algo/common.h>

constexpr long long c_div(long long a, long long b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr long long f_div(long long a, long long b) { return a / b - ((a ^ b) < 0 && a % b); }
