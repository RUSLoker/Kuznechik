#include <assert.h>
#include <constants.h>
#include <funcs.hpp>
#include <stdio.h>
#include <string.h>

struct F delta(uint8_t byte) {
    struct F result;

    static_assert(sizeof(result) == sizeof(byte));
    memcpy(&result, &byte, sizeof(result));
    return result;
}

uint8_t reverce_delta(struct F f) {
    uint8_t result;

    static_assert(sizeof(result) == sizeof(f));
    memcpy(&result, &f, sizeof(result));
    return result;
}

struct F f_add(struct F lhs, struct F rhs) {
    return delta(reverce_delta(lhs) ^ reverce_delta(rhs));
}

struct ExtendedF {
    uint8_t z_0: 1;
    uint8_t z_1: 1;
    uint8_t z_2: 1;
    uint8_t z_3: 1;
    uint8_t z_4: 1;
    uint8_t z_5: 1;
    uint8_t z_6: 1;
    uint8_t z_7: 1;
    uint8_t z_8: 1;
    uint8_t z_9: 1;
    uint8_t z_10: 1;
    uint8_t z_11: 1;
    uint8_t z_12: 1;
    uint8_t z_13: 1;
    uint8_t z_14: 1;
    uint8_t z_15: 1;
};

void print_f(struct F f) {
    printf(
        "(%i %i %i %i %i %i %i %i)",
        f.z_7,
        f.z_6,
        f.z_5,
        f.z_4,
        f.z_3,
        f.z_2,
        f.z_1,
        f.z_0
    );
}

void print_ext_f(struct ExtendedF ext_f) {
    printf(
        "(%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i)",
        ext_f.z_14,
        ext_f.z_13,
        ext_f.z_12,
        ext_f.z_11,
        ext_f.z_10,
        ext_f.z_9,
        ext_f.z_8,
        ext_f.z_7,
        ext_f.z_6,
        ext_f.z_5,
        ext_f.z_4,
        ext_f.z_3,
        ext_f.z_2,
        ext_f.z_1,
        ext_f.z_0
    );
}

struct ExtendedF ext_delta(uint16_t word) {
    struct ExtendedF result;

    static_assert(sizeof(result) == sizeof(word));
    memcpy(&result, &word, sizeof(result));
    return result;
}

uint16_t ext_reverce_delta(struct ExtendedF ext_f) {
    uint16_t result;

    static_assert(sizeof(result) == sizeof(ext_f));
    memcpy(&result, &ext_f, sizeof(result));
    return result;
}

struct ExtendedF ext_f_add(struct ExtendedF lhs, struct ExtendedF rhs) {
    return ext_delta(ext_reverce_delta(lhs) ^ ext_reverce_delta(rhs));
}

/**
 *
 * @param lhs
 * @param rhs power of z
 * @return
 */
struct ExtendedF ext_f_mul_x_n(struct ExtendedF lhs, uint8_t rhs) {
    return ext_delta(ext_reverce_delta(lhs) << rhs);
}

uint8_t fls(int x) {
    return sizeof(x) * 8 - __builtin_clz(x);
}

struct ExtendedF ext_f_div_rem(struct ExtendedF lhs, struct ExtendedF rhs) {
    if (ext_reverce_delta(rhs) == 0) return lhs;

    uint8_t highest_bit_rhs = fls(ext_reverce_delta(rhs));

    for (uint8_t i = fls(ext_reverce_delta(lhs)); i >= highest_bit_rhs; i = fls(ext_reverce_delta(lhs))) {
        uint8_t power = i - highest_bit_rhs;
        lhs = ext_f_add(lhs, ext_f_mul_x_n(rhs, power));
    }

    return lhs;
}

struct F ext_f_to_f(struct ExtendedF ext_f) {
    /* p(x) = x^8 + x^7 + x^6 + x + 1 ∈ GF(2)[x]; */
    const struct ExtendedF p_x = {
        .z_0 = 1,
        .z_1 = 1,
        .z_2 = 0,
        .z_3 = 0,
        .z_4 = 0,
        .z_5 = 0,
        .z_6 = 1,
        .z_7 = 1,
        .z_8 = 1,
        .z_9 = 0,
        .z_10 = 0,
        .z_11 = 0,
        .z_12 = 0,
        .z_13 = 0,
        .z_14 = 0,
        .z_15 = 0
    };

    struct ExtendedF ext_res = ext_f_div_rem(ext_f, p_x);

    struct F result;

    static_assert(sizeof(ext_res) == 2 * sizeof(result));
    memcpy(&result, &ext_res, sizeof(result));

    return result;
}

struct F f_mul(struct F lhs, struct F rhs) {
    struct ExtendedF result = {
        .z_0 = (lhs.z_0 & rhs.z_0),
        .z_1 = (lhs.z_0 & rhs.z_1) ^ (lhs.z_1 & rhs.z_0),
        .z_2 = (lhs.z_0 & rhs.z_2) ^ (lhs.z_1 & rhs.z_1) ^ (lhs.z_2 & rhs.z_0),
        .z_3 = (lhs.z_0 & rhs.z_3) ^ (lhs.z_1 & rhs.z_2) ^ (lhs.z_2 & rhs.z_1) ^ (lhs.z_3 & rhs.z_0),
        .z_4 = (lhs.z_0 & rhs.z_4) ^ (lhs.z_1 & rhs.z_3) ^ (lhs.z_2 & rhs.z_2) ^ (lhs.z_3 & rhs.z_1) ^ (lhs.z_4 & rhs.z_0),
        .z_5 = (lhs.z_0 & rhs.z_5) ^ (lhs.z_1 & rhs.z_4) ^ (lhs.z_2 & rhs.z_3) ^ (lhs.z_3 & rhs.z_2) ^ (lhs.z_4 & rhs.z_1) ^ (lhs.z_5 & rhs.z_0),
        .z_6 = (lhs.z_0 & rhs.z_6) ^ (lhs.z_1 & rhs.z_5) ^ (lhs.z_2 & rhs.z_4) ^ (lhs.z_3 & rhs.z_3) ^ (lhs.z_4 & rhs.z_2) ^ (lhs.z_5 & rhs.z_1) ^ (lhs.z_6 & rhs.z_0),
        .z_7 = (lhs.z_0 & rhs.z_7) ^ (lhs.z_1 & rhs.z_6) ^ (lhs.z_2 & rhs.z_5) ^ (lhs.z_3 & rhs.z_4) ^ (lhs.z_4 & rhs.z_3) ^ (lhs.z_5 & rhs.z_2) ^ (lhs.z_6 & rhs.z_1) ^ (lhs.z_7 & rhs.z_0),
        .z_8 = (lhs.z_1 & rhs.z_7) ^ (lhs.z_2 & rhs.z_6) ^ (lhs.z_3 & rhs.z_5) ^ (lhs.z_4 & rhs.z_4) ^ (lhs.z_5 & rhs.z_3) ^ (lhs.z_6 & rhs.z_2) ^ (lhs.z_7 & rhs.z_1),
        .z_9 = (lhs.z_2 & rhs.z_7) ^ (lhs.z_3 & rhs.z_6) ^ (lhs.z_4 & rhs.z_5) ^ (lhs.z_5 & rhs.z_4) ^ (lhs.z_6 & rhs.z_3) ^ (lhs.z_7 & rhs.z_2),
        .z_10 = (lhs.z_3 & rhs.z_7) ^ (lhs.z_4 & rhs.z_6) ^ (lhs.z_5 & rhs.z_5) ^ (lhs.z_6 & rhs.z_4) ^ (lhs.z_7 & rhs.z_3),
        .z_11 = (lhs.z_4 & rhs.z_7) ^ (lhs.z_5 & rhs.z_6) ^ (lhs.z_6 & rhs.z_5) ^ (lhs.z_7 & rhs.z_4),
        .z_12 = (lhs.z_5 & rhs.z_7) ^ (lhs.z_6 & rhs.z_6) ^ (lhs.z_7 & rhs.z_5),
        .z_13 = (lhs.z_6 & rhs.z_7) ^ (lhs.z_7 & rhs.z_6),
        .z_14 = (lhs.z_7 & rhs.z_7),
        .z_15 = 0
    };

    return ext_f_to_f(result);
}

uint8_t linear_transform(const uint8_t a[16]) {
    const uint8_t COEFS[16] = {1, 148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148};

    struct F result = {0};

    for (uint8_t i = 0; i < 16; ++i) {
        result = f_add(result, f_mul(delta(COEFS[i]), delta(a[i])));
    }

    return reverce_delta(result);
}

unsigned __int128 s_transform(unsigned __int128 a) {
    uint8_t *a_ptr = (uint8_t*)&a;
    for (uint8_t i = 0; i < sizeof(a); ++i) {
        a_ptr[i] = pi[a_ptr[i]];
    }
    return a;
}

unsigned __int128 reverce_s_transform(unsigned __int128 a) {
    uint8_t *a_ptr = (uint8_t*)&a;
#pragma unroll
    for (uint8_t i = 0; i < sizeof(a); ++i) {
        a_ptr[i] = reverse_pi[a_ptr[i]];
    }
    return a;
}

unsigned __int128 r_transform(unsigned __int128 a) {
    uint8_t *a_ptr = (uint8_t*)&a;
    uint8_t new_byte = linear_transform(a_ptr);
    a >>= 8;
    a_ptr[15] = new_byte;
    return a;
}

unsigned __int128 reverce_r_transform(unsigned __int128 a) {
    uint8_t *a_ptr = (uint8_t *)&a;
    uint8_t rotated_a[16] = {
        a_ptr[15], a_ptr[0], a_ptr[1], a_ptr[2], a_ptr[3], a_ptr[4], a_ptr[5], a_ptr[6], a_ptr[7], a_ptr[8],
        a_ptr[9], a_ptr[10], a_ptr[11], a_ptr[12], a_ptr[13], a_ptr[14]
    };

    a <<= 8;

    a_ptr[0] = linear_transform(rotated_a);

    return a;
}

unsigned __int128 l_transform(unsigned __int128 a) {
#pragma unroll
    for (uint8_t _ = 0; _ < 16; ++_) {
        a = r_transform(a);
    }
    return a;
}

unsigned __int128 reverce_l_transform(unsigned __int128 a) {
#pragma unroll
    for (uint8_t _ = 0; _ < 16; ++_) {
        a = reverce_r_transform(a);
    }
    return a;
}

unsigned __int128 x_transform(unsigned __int128 k, unsigned __int128 a) {
    return k ^ a;
}

struct pair_uint128 f_transform(unsigned __int128 k, struct pair_uint128 a) {
    struct pair_uint128 result = {l_transform(s_transform(x_transform(k, a.first))) ^ a.second, a.first};
    return result;
}

void key_shedule(struct pair_uint128 key, unsigned __int128 K[10]) {
    unsigned __int128 C[32];
    for (unsigned __int128 i = 0; i < 32; ++i) {
        C[i] = l_transform(i + 1);
    }

    K[0] = key.first;
    K[1] = key.second;
    for (uint8_t i = 1; i <= 4; ++i) {
        struct pair_uint128 result = {K[2 * i - 2], K[2 * i - 1]};
        for (uint8_t j = 0; j < 8; ++j) {
            result = f_transform(C[8 * (i - 1) + j], result);
        }
        K[2 * i] = result.first;
        K[2 * i + 1] = result.second;
    }
}

unsigned __int128 encrypt(struct pair_uint128 key, unsigned __int128 a) {
    unsigned __int128 K[10];
    key_shedule(key, K);

    a = x_transform(K[0], a);
    for (uint8_t i = 1; i < 10; ++i) {
        a = x_transform(K[i], l_transform(s_transform(a)));
    }
    return a;
}

unsigned __int128 decrypt(struct pair_uint128 key, unsigned __int128 a) {
    unsigned __int128 K[10];
    key_shedule(key, K);

    a = x_transform(K[9], a);
    for (int8_t i = 8; i >= 0; --i) {
        a = x_transform(K[i], reverce_s_transform(reverce_l_transform(a)));
    }
    return a;
}
