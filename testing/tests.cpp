#include <funcs.hpp>
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
    }
    return y;
}

TEST(Transforms, S) {
    ASSERT_EQ(s_transform(0xffeeddccbbaa99881122334455667700_uint128), 0xb66cd8887d38e8d77765aeea0c9a7efc_uint128);
    ASSERT_EQ(s_transform(0xb66cd8887d38e8d77765aeea0c9a7efc_uint128), 0x559d8dd7bd06cbfe7e7b262523280d39_uint128);
    ASSERT_EQ(s_transform(0x559d8dd7bd06cbfe7e7b262523280d39_uint128), 0x0c3322fed531e4630d80ef5c5a81c50b_uint128);
    ASSERT_EQ(s_transform(0x0c3322fed531e4630d80ef5c5a81c50b_uint128), 0x23ae65633f842d29c5df529c13f5acda_uint128);
}

TEST(Transforms, R) {
    ASSERT_EQ(r_transform(0x00000000000000000000000000000100_uint128), 0x94000000000000000000000000000001_uint128);
    ASSERT_EQ(r_transform(0x94000000000000000000000000000001_uint128), 0xa5940000000000000000000000000000_uint128);
    ASSERT_EQ(r_transform(0xa5940000000000000000000000000000_uint128), 0x64a59400000000000000000000000000_uint128);
    ASSERT_EQ(r_transform(0x64a59400000000000000000000000000_uint128), 0x0d64a594000000000000000000000000_uint128);
}

TEST(Transforms, L) {
    ASSERT_EQ(l_transform(0x64a59400000000000000000000000000_uint128), 0xd456584dd0e3e84cc3166e4b7fa2890d_uint128);
    ASSERT_EQ(l_transform(0xd456584dd0e3e84cc3166e4b7fa2890d_uint128), 0x79d26221b87b584cd42fbc4ffea5de9a_uint128);
    ASSERT_EQ(l_transform(0x79d26221b87b584cd42fbc4ffea5de9a_uint128), 0x0e93691a0cfc60408b7b68f66b513c13_uint128);
    ASSERT_EQ(l_transform(0x0e93691a0cfc60408b7b68f66b513c13_uint128), 0xe6a8094fee0aa204fd97bcb0b44b8580_uint128);
}

TEST(KeyShedule, 1) {
    pair_uint128 key = {0x8899aabbccddeeff0011223344556677_uint128, 0xfedcba98765432100123456789abcdef_uint128};
    __uint128_t K[10];
    key_shedule(key, K);

    __uint128_t real_K[10] = {
        0x8899aabbccddeeff0011223344556677_uint128,
        0xfedcba98765432100123456789abcdef_uint128,
        0xdb31485315694343228d6aef8cc78c44_uint128,
        0x3d4553d8e9cfec6815ebadc40a9ffd04_uint128,
        0x57646468c44a5e28d3e59246f429f1ac_uint128,
        0xbd079435165c6432b532e82834da581b_uint128,
        0x51e640757e8745de705727265a0098b1_uint128,
        0x5a7925017b9fdd3ed72a91a22286f984_uint128,
        0xbb44e25378c73123a5f32f73cdb6e517_uint128,
        0x72e9dd7416bcf45b755dbaa88e4a4043_uint128
    };

    ASSERT_EQ(real_K[0], K[0]);
    ASSERT_EQ(real_K[1], K[1]);
    ASSERT_EQ(real_K[2], K[2]);
    ASSERT_EQ(real_K[3], K[3]);
    ASSERT_EQ(real_K[4], K[4]);
    ASSERT_EQ(real_K[5], K[5]);
    ASSERT_EQ(real_K[6], K[6]);
    ASSERT_EQ(real_K[7], K[7]);
    ASSERT_EQ(real_K[8], K[8]);
    ASSERT_EQ(real_K[9], K[9]);
}

TEST(Encrypt, 1) {
    pair_uint128 key = {0x8899aabbccddeeff0011223344556677_uint128, 0xfedcba98765432100123456789abcdef_uint128};
    ASSERT_EQ(encrypt(key, 0x1122334455667700ffeeddccbbaa9988_uint128), 0x7f679d90bebc24305a468d42b9d4edcd_uint128);
}

TEST(Decrypt, 1) {
    pair_uint128 key = {0x8899aabbccddeeff0011223344556677_uint128, 0xfedcba98765432100123456789abcdef_uint128};
    ASSERT_EQ(decrypt(key, 0x7f679d90bebc24305a468d42b9d4edcd_uint128), 0x1122334455667700ffeeddccbbaa9988_uint128);
}
