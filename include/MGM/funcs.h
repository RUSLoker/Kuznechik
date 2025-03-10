#pragma once

#include <funcs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

pair_uint128 multiply_as_polynomials(__uint128_t a, __uint128_t b);

pair_uint128 poly256_remainder(pair_uint128 a, pair_uint128 b);

__uint128_t get_gost_remainder_poly256(pair_uint128 poly);

block_t multyply_blocks(block_t a, block_t b);

#ifdef __cplusplus
}
#endif
