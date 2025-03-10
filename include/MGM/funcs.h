#pragma once

#include <funcs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

pair_uint128 multiply_as_polynoms(__uint128_t a, __uint128_t b);

pair_uint128 poly256_remainder(pair_uint128 a, pair_uint128 b);

#ifdef __cplusplus
}
#endif
