#pragma once
#include <stdint.h>

/**
 * Конечное поле GF(2)[x]/p(x), где
 * p(x) = x^8 + x^7 + x^6 + x + 1 ∈ GF(2)[x];
 */
struct F {
    uint8_t z_0: 1;
    uint8_t z_1: 1;
    uint8_t z_2: 1;
    uint8_t z_3: 1;
    uint8_t z_4: 1;
    uint8_t z_5: 1;
    uint8_t z_6: 1;
    uint8_t z_7: 1;
};

#ifdef __cplusplus
extern "C" {
#endif

struct F delta(uint8_t);

uint8_t reverce_delta(struct F);

struct F f_add(struct F lhs, struct F rhs);

struct F f_mul(struct F lhs, struct F rhs);

uint8_t fls(int x);

uint8_t linear_transform(const uint8_t a[16]);

unsigned __int128 s_transform(unsigned __int128 a);

unsigned __int128 reverce_s_transform(unsigned __int128 a);

unsigned __int128 r_transform(unsigned __int128 a);

unsigned __int128 reverce_r_transform(unsigned __int128 a);

unsigned __int128 l_transform(unsigned __int128 a);

unsigned __int128 reverce_l_transform(unsigned __int128 a);

unsigned __int128 x_transform(unsigned __int128 k, unsigned __int128 a);

struct pair_uint128 {
    unsigned __int128 first;
    unsigned __int128 second;
};

struct pair_uint128 f_transform(unsigned __int128 k, struct pair_uint128 a);

void key_shedule(struct pair_uint128 key, unsigned __int128 K[10]);

unsigned __int128 encrypt(struct pair_uint128 key, unsigned __int128 a);

unsigned __int128 decrypt(struct pair_uint128 key, unsigned __int128 a);

#ifdef __cplusplus
}
#endif

#define INT128(high, low) (((__int128)(high) << 64) | (low))
