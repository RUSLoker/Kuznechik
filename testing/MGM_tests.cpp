#include <modes/MGM.h>
#include <gtest/gtest.h>

constexpr __uint128_t operator""_uint128(const char *x) {
    __uint128_t y = 0;
    for (int i = 2; x[i] != '\0'; ++i) {
        y *= 16ull;
        if ('0' <= x[i] && x[i] <= '9')
            y += x[i] - '0';
        else if ('A' <= x[i] && x[i] <= 'F')
            y += x[i] - 'A' + 10;
        else if ('a' <= x[i] && x[i] <= 'f')
            y += x[i] - 'a' + 10;
        else
            assert(false);
    }
    return y;
}

constexpr key256_t operator""_key256_t(const char *x) {
    key256_t y = {(__uint128_t)0, (__uint128_t)0};
    __uint128_t *cur = &y.first;
    uint64_t i = 2;
    for (; x[i] != '\0'; ++i) {
        if (i == 34) {
            cur = &y.second;
        }

        *cur *= 16ull;
        if ('0' <= x[i] && x[i] <= '9')
            *cur += x[i] - '0';
        else if ('A' <= x[i] && x[i] <= 'F')
            *cur += x[i] - 'A' + 10;
        else if ('a' <= x[i] && x[i] <= 'f')
            *cur += x[i] - 'a' + 10;
        else
            assert(false);
    }
    return y;
}

TEST(MGM, 1) {
    key256_t key = 0x8899AABBCCDDEEFF0011223344556677FEDCBA98765432100123456789ABCDEF_key256_t;
    block_t nonce = 0x1122334455667700FFEEDDCCBBAA9988_uint128;

    bit_vec_t additional_data = make_bit_vec(326);
    additional_data.data[0] = 0x02020202020202020101010101010101_uint128;
    additional_data.data[1] = 0x04040404040404040303030303030303_uint128;
    additional_data.data[2] = 0xEA050505050505050500000000000000_uint128;

    bit_vec_t plain_text = make_bit_vec(536);
    plain_text.data[0] = 0x1122334455667700FFEEDDCCBBAA9988_uint128;
    plain_text.data[1] = 0x00112233445566778899AABBCCEEFF0A_uint128;
    plain_text.data[2] = 0x112233445566778899AABBCCEEFF0A00_uint128;
    plain_text.data[3] = 0x2233445566778899AABBCCEEFF0A0011_uint128;
    plain_text.data[4] = 0xAABBCC00000000000000000000000000_uint128;

    uint64_t MAC_size = 128;

    MGM_result result = MGM_Encrypt(key, nonce, additional_data, plain_text, MAC_size);

    bit_vec_t expected_cyphered_text = make_bit_vec(640);
    expected_cyphered_text.data[0] = 0xA9757B8147956E9055B8A33DE89F42FC_uint128;
    expected_cyphered_text.data[1] = 0x8075D2212BF9FD5BD3F7069AADC16B39_uint128;
    expected_cyphered_text.data[2] = 0x497AB15915A6BA85936B5D0EA9F6851C_uint128;
    expected_cyphered_text.data[3] = 0xC60C14D4D3F883D0AB94420695C76DEB_uint128;
    expected_cyphered_text.data[4] = 0x2C755200000000000000000000000000_uint128;

    ASSERT_GE(result.cyphered_text.alloc_len, expected_cyphered_text.alloc_len);
    EXPECT_EQ(result.cyphered_text.bit_size, expected_cyphered_text.bit_size);
    for (uint64_t i = 0; i < expected_cyphered_text.alloc_len; ++i) {
        EXPECT_EQ(result.cyphered_text.data[i], expected_cyphered_text.data[i]);
    }

    bit_vec_t expected_additional_data = make_bit_vec(384);
    expected_additional_data.data[0] = 0x02020202020202020101010101010101_uint128;
    expected_additional_data.data[1] = 0x04040404040404040303030303030303_uint128;
    expected_additional_data.data[2] = 0xEA050505050505050500000000000000_uint128;


    ASSERT_GE(result.additional_data.alloc_len, expected_additional_data.alloc_len);
    EXPECT_EQ(result.additional_data.bit_size, expected_additional_data.bit_size);
    for (uint64_t i = 0; i < expected_additional_data.alloc_len; ++i) {
        EXPECT_EQ(result.additional_data.data[i], expected_additional_data.data[i]);
    }

    block_t expected_MAC = 0xCF5D656F40C34F5C46E8BB0E29FCDB4C_uint128;
    EXPECT_EQ(result.MAC.bit_size, (sizeof(block_t) * 8));
    EXPECT_EQ(result.MAC.data[0], expected_MAC);
}
