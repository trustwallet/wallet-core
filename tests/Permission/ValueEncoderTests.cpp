// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Permission/ABI/ValueEncoder.h"
#include <HexCoding.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Permission;

Data dataPermissiion;

void checkLast32BytesEqualPermission(const Data& dataPermissiion, const char* expected) {
    EXPECT_EQ(hex(subData(dataPermissiion, dataPermissiion.size() - 32, 32)), expected);
}
TEST(PermissionAbiValueEncoder, encodeBool) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeBool(false, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeBool(true, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000001");
}

TEST(PermissionAbiValueEncoder, encodeInt32) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeInt32(69, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeInt32(-1, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    ABI::ValueEncoder::encodeInt32(0, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeInt32(1, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000001");

    ABI::ValueEncoder::encodeUInt32(69, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeUInt32(0, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeUInt32(1, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000001");
}

TEST(PermissionAbiValueEncoder, encodeUInt256) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeInt256(69, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeInt256(0, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeInt256(1, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000001");

    ABI::ValueEncoder::encodeUInt256(69, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeUInt256(0, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeUInt256(1, dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000000000000000000000000000000000000000000001");
}

TEST(PermissionAbiValueEncoder, encodeUInt256BigIntOverflow) {
    Data encoded;
    try {
        ABI::ValueEncoder::encodeUInt256(uint256_t(int256_t("F123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0")), encoded);
    } catch (std::exception& ex) {
        // expected exception
        return;
    }
    FAIL() << "Should fail due to overflow";
}

TEST(PermissionAbiValueEncoder, encodeAddress) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeAddress(parse_hex("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"), dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "0000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed");
}

TEST(PermissionAbiValueEncoder, encodeString) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeString("trustwallet", dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}

TEST(PermissionAbiValueEncoder, encodeBytes) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeBytes(parse_hex("45"), dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "4500000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeBytes(parse_hex("5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"), dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "5aaeb6053f3e94c9b9a09f33669435e7ef1beaed000000000000000000000000");
    ABI::ValueEncoder::encodeBytes(parse_hex("000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f"), dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f");
    // too long, truncated
    ABI::ValueEncoder::encodeBytes(parse_hex("000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f3333"), dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f");
}

TEST(PermissionAbiValueEncoder, encodeBytesDyn) {
    Data dataPermissiion;
    ABI::ValueEncoder::encodeBytesDyn(TW::data(std::string("trustwallet")), dataPermissiion);
    checkLast32BytesEqualPermission(dataPermissiion, "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}

TEST(PermissionAbiValueEncoder, int256FromUint256) {
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(0), 0);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(1), 1);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(100), 100);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(~uint256_t(0)), -1);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(~uint256_t(1)), -2);
}

TEST(PermissionAbiValueEncoder, uint256FromInt256) {
    Data dataPermissiion;
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(0), 0);
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(1), 1);
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(100), 100);
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(-1), (~uint256_t(0)));
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(-2), (~uint256_t(1)));
}

TEST(PermissionAbiValueEncoder, pad32) {
    EXPECT_EQ(64, ABI::ValueEncoder::paddedTo32(40));
    EXPECT_EQ(32, ABI::ValueEncoder::paddedTo32(32));
    EXPECT_EQ(64, ABI::ValueEncoder::paddedTo32(33));
    EXPECT_EQ(64, ABI::ValueEncoder::paddedTo32(63));
    EXPECT_EQ(64, ABI::ValueEncoder::paddedTo32(64));
    EXPECT_EQ(96, ABI::ValueEncoder::paddedTo32(65));
    EXPECT_EQ(24, ABI::ValueEncoder::padNeeded32(40));
    EXPECT_EQ(0, ABI::ValueEncoder::padNeeded32(32));
    EXPECT_EQ(31, ABI::ValueEncoder::padNeeded32(33));
    EXPECT_EQ(1, ABI::ValueEncoder::padNeeded32(63));
    EXPECT_EQ(0, ABI::ValueEncoder::padNeeded32(64));
    EXPECT_EQ(31, ABI::ValueEncoder::padNeeded32(65));
}
