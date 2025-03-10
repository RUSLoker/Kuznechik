#include <funcs.h>
#include <stdio.h>
#include <MGM/funcs.h>

void print_128(__uint128_t value) {
    uint64_t lo = value;
    uint64_t hi = (value >> 64);
    printf("%016llx%016llx\n", hi, lo);
}

void print_256(pair_uint128 value) {
    uint64_t lo0 = value.first;
    uint64_t hi0 = (value.first >> 64);
    uint64_t lo1 = value.second;
    uint64_t hi1 = (value.second >> 64);
    printf("%016llx%016llx %016llx%016llx\n", hi1, lo1, hi0, lo0);
}

int main(void) {
    __uint128_t a = ((__uint128_t)0b1ULL << 127) + 1;
    __uint128_t b = ((__uint128_t)0b1ULL << 127) + 1;

    print_128(a);
    print_128(b);

    pair_uint128 result = multiply_as_polynomials(a, b);

    // const __m256i p = _mm256_set_epi64x(0, 0, 0, );

    print_256(result);

    int x = 0b111100101000 % 0b111000011;

    printf("%i", x);

    return 0;
}
