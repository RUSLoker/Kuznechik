#include <MGM/funcs.h>
#include <stdint.h>

pair_uint128 multiply_as_polynomials(const __uint128_t a, const __uint128_t b) {
    pair_uint128 result = {0, 0};

    // 0 iteration
    {
        __uint128_t mask = -(b & 1);

        result.first ^= a & mask;
    }

    for (__uint128_t i = 1; i < 128; ++i) {
        // iterate over each bit of b
        __uint128_t mask = -((b >> i) & 1);

        result.first ^= (a << i) & mask;
        result.second ^= (a >> (128 - i)) & mask;
    }

    return result;
}

static inline uint64_t int128_high(__uint128_t val) {
    return val >> 64;
}

static inline uint64_t int128_low(__uint128_t val) {
    return val;
}

// Function to compute the degree of a polynomial
static inline int poly_degree(pair_uint128 p) {
    if (int128_high(p.second) != 0) {
        return 255 - __builtin_clzll(int128_high(p.second));
    }
    if (int128_low(p.second) != 0) {
        return 191 - __builtin_clzll(int128_low(p.second));
    }
    if (int128_high(p.first) != 0) {
        return 127 - __builtin_clzll(int128_high(p.first));
    }
    if (int128_low(p.first) != 0) {
        return 63 - __builtin_clzll(int128_low(p.first));
    }
    return -1; // Zero polynomial
}

// Left shift the polynomial by 'shift' bits
static inline pair_uint128 poly_shift_left(pair_uint128 p, int shift) {
    if (shift >= 128) {
        return (pair_uint128){.first = 0, .second = p.first << (shift - 128)};
    } else {
        return (pair_uint128){
            .first = p.first << shift,
            .second = (p.second << shift) | (p.first >> (128 - shift))
        };
    }
}

// XOR of two polynomials
static inline pair_uint128 poly_xor(pair_uint128 a, pair_uint128 b) {
    return (pair_uint128){.first = a.first ^ b.first, .second = a.second ^ b.second};
}

// Function to compute the remainder of polynomial division a / b
pair_uint128 poly256_remainder(pair_uint128 a, pair_uint128 b) {
    int deg_a = poly_degree(a);
    int deg_b = poly_degree(b);

    if (deg_a < deg_b) {
        return a; // a is already the remainder
    }

    while (deg_a >= deg_b) {
        int shift = deg_a - deg_b;
        a = poly_xor(a, poly_shift_left(b, shift));
        deg_a = poly_degree(a);
    }

    return a;
}

unsigned __int128 get_gost_remainder_poly256(pair_uint128 poly) {
    pair_uint128 gost_remainder = {
        .first = (1 << 7) | (1 << 2) | (1 << 1) | 1,
        .second = 1
    };
    return poly256_remainder(poly, gost_remainder).first;
}

block_t multyply_blocks(block_t a, block_t b) {
    return get_gost_remainder_poly256(multiply_as_polynomials(a, b));
}
