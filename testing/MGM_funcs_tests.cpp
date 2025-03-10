#include <modes/MGM.h>
#include <MGM/funcs.h>
#include <gtest/gtest.h>

TEST(polynomial_multiplication, 1) {
    __uint128_t a = __uint128_t(1) << 127 | __uint128_t(1);
    __uint128_t b = __uint128_t(1) << 127 | __uint128_t(1);

    const pair_uint128 result = multiply_as_polynoms(a, b);

    pair_uint128 right_result{__uint128_t(1), __uint128_t(1) << 126};

    EXPECT_EQ(result.first, right_result.first);
    EXPECT_EQ(result.second, right_result.second);
}

TEST(polynomial_multiplication, 2) {
    __uint128_t a = __uint128_t(1) << 127 | __uint128_t(1) << 31 | __uint128_t(1) << 5 | __uint128_t(1);
    __uint128_t b = __uint128_t(1) << 127 | __uint128_t(1) << 34 | __uint128_t(1) << 29 | __uint128_t(1) << 3;

    const pair_uint128 result = multiply_as_polynoms(a, b);

    pair_uint128 right_result{
        __uint128_t(1) << 3 |
        __uint128_t(1) << 8 |
        __uint128_t(1) << 29 |
        __uint128_t(1) << 34 |
        __uint128_t(1) << 39 |
        __uint128_t(1) << 60 |
        __uint128_t(1) << 65 |
        __uint128_t(1) << 127,
        __uint128_t(1) << (130 - 128) |
        __uint128_t(1) << (132 - 128) |
        __uint128_t(1) << (156 - 128) |
        __uint128_t(1) << (158 - 128) |
        __uint128_t(1) << (161 - 128) |
        __uint128_t(1) << (254 - 128)
    };

    EXPECT_EQ(result.first, right_result.first);
    EXPECT_EQ(result.second, right_result.second);
}

TEST(polynomial_remainder, 1) {
    pair_uint128 a = {
        .first = (1 << 3) | (1 << 5) | (1 << 8) | (1 << 9) | (1 << 10) | (1 << 11),
        .second = 0
    };
    pair_uint128 b = {
        .first = 1 | (1 << 1) | (1 << 6) | (1 << 7) | (1 << 8),
        .second = 0
    };

    pair_uint128 result = poly256_remainder(a, b);

    pair_uint128 right_result = {
        .first = 1 | (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7),
        .second = 0
    };

    EXPECT_EQ(result.first, right_result.first);
    EXPECT_EQ(result.second, right_result.second);
}

TEST(polynomial_remainder, 2) {
    pair_uint128 a, b, r, expected;

    a.first = ((__uint128_t)1 << 2) | ((__uint128_t)1 << 1);
    a.second = 0;
    b.first = (__uint128_t)1 << 3;
    b.second = 0;
    expected = a;
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}

TEST(polynomial_remainder, 3) {
    pair_uint128 a, b, r, expected;

    a.first = ((__uint128_t)1 << 7) | ((__uint128_t)1 << 3);
    a.second = 0;
    b = a;
    expected.first = 0;
    expected.second = 0;
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}

TEST(polynomial_remainder, 4) {
    pair_uint128 a, b, r, expected;

    a.first = 0; a.second = 0;
    b.first = ((__uint128_t)1 << 4) | ((__uint128_t)1 << 2);
    b.second = 0;
    expected = a;
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}

TEST(polynomial_remainder, 5) {
    pair_uint128 a, b, r, expected;

    // Dividing by constant polynomial 1 always yields zero remainder.
    a.first = ((__uint128_t)1 << 10) | 1;
    a.second = 0;
    b.first = 1;
    b.second = 0;
    expected.first = 0;
    expected.second = 0;
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}

TEST(polynomial_remainder, high_1) {
    pair_uint128 a, b, r, expected;

    // a(x)= x^200 + x^150 + x^20, b(x)= x^128+1, remainder: x^72+x^22+x^20.
    a.first = ((__uint128_t) 1 << 20);
    a.second = (((__uint128_t) 1 << 22) | ((__uint128_t) 1 << 72));
    b.first = 1;
    b.second = 1;
    expected.first = (((__uint128_t) 1 << 20) | ((__uint128_t) 1 << 22) | ((__uint128_t) 1 << 72));
    expected.second = 0;
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}

TEST(polynomial_remainder, high_2) {
    pair_uint128 a, b, r, expected;

    // a(x)= x^250 + x^240 + x^230, b(x)= x^245 + x^130, remainder: x^240+x^230+x^135.
    a.first = 0;
    a.second = (((__uint128_t) 1 << 122) | ((__uint128_t) 1 << 112) | ((__uint128_t) 1 << 102));
    b.first = 0;
    b.second = (((__uint128_t) 1 << 117) | (((__uint128_t) 1) << 2));
    expected.first = 0;
    expected.second = (((__uint128_t) 1 << 112) | ((__uint128_t) 1 << 102) | (((__uint128_t) 1) << 7));
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}

TEST(polynomial_remainder, high_3) {
    pair_uint128 a, b, r, expected;

    // a(x)= x^255 + x^130, b(x)= x^254 + 1, remainder: x^130 + x^1.
    a.first = 0;
    a.second = (((__uint128_t) 1 << 127) | (((__uint128_t) 1) << 2));
    b.first = 1;
    b.second = (((__uint128_t) 1) << 126);
    expected.first = ((__uint128_t) 1 << 1);
    expected.second = (((__uint128_t) 1) << 2);
    r = poly256_remainder(a, b);
    EXPECT_EQ(r.first, expected.first);
    EXPECT_EQ(r.second, expected.second);
}
