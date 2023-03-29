// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/ABI/ValueEncoder.h"
#include <HexCoding.h>

#include <gtest/gtest.h>

namespace TW::Ethereum::tests {

void checkLast32BytesEqual(const Data& data, const char* expected) {
    EXPECT_EQ(hex(subData(data, data.size() - 32, 32)), expected);
}

TEST(EthereumAbiValueEncoder, encodeBool) {
    Data data;
    ABI::ValueEncoder::encodeBool(false, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeBool(true, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000001");
}

TEST(EthereumAbiValueEncoder, encodeInt32) {
    Data data;
    ABI::ValueEncoder::encodeInt32(69, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeInt32(-1, data);
    checkLast32BytesEqual(data, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    ABI::ValueEncoder::encodeInt32(0, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeInt32(1, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000001");

    ABI::ValueEncoder::encodeUInt32(69, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeUInt32(0, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeUInt32(1, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000001");
}

TEST(EthereumAbiValueEncoder, encodeUInt256) {
    Data data;
    ABI::ValueEncoder::encodeInt256(69, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeInt256(0, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeInt256(1, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000001");

    ABI::ValueEncoder::encodeUInt256(69, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000045");
    ABI::ValueEncoder::encodeUInt256(0, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeUInt256(1, data);
    checkLast32BytesEqual(data, "0000000000000000000000000000000000000000000000000000000000000001");
}

TEST(EthereumAbiValueEncoder, encodeUInt256BigIntOverflow) {
    Data encoded;
    try {
        ABI::ValueEncoder::encodeUInt256(uint256_t(int256_t("F123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0")), encoded);
    } catch (std::exception& ex) {
        // expected exception
        return;
    }
    FAIL() << "Should fail due to overflow";
}

TEST(EthereumAbiValueEncoder, encodeAddress) {
    Data data;
    ABI::ValueEncoder::encodeAddress(parse_hex("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"), data);
    checkLast32BytesEqual(data, "0000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed");
}

TEST(EthereumAbiValueEncoder, encodeString) {
    Data data;
    ABI::ValueEncoder::encodeString("trustwallet", data);
    checkLast32BytesEqual(data, "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}

TEST(EthereumAbiValueEncoder, encodeBytes) {
    Data data;
    ABI::ValueEncoder::encodeBytes(parse_hex("45"), data);
    checkLast32BytesEqual(data, "4500000000000000000000000000000000000000000000000000000000000000");
    ABI::ValueEncoder::encodeBytes(parse_hex("5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"), data);
    checkLast32BytesEqual(data, "5aaeb6053f3e94c9b9a09f33669435e7ef1beaed000000000000000000000000");
    ABI::ValueEncoder::encodeBytes(parse_hex("000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f"), data);
    checkLast32BytesEqual(data, "000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f");
    // too long, truncated
    ABI::ValueEncoder::encodeBytes(parse_hex("000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f3333"), data);
    checkLast32BytesEqual(data, "000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f");
}

TEST(EthereumAbiValueEncoder, encodeBytesDyn) {
    Data data;
    ABI::ValueEncoder::encodeBytesDyn(TW::data(std::string("trustwallet")), data);
    checkLast32BytesEqual(data, "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}

TEST(EthereumAbiValueEncoder, int256FromUint256) {
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(0), 0);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(1), 1);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(100), 100);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(~uint256_t(0)), -1);
    EXPECT_EQ(ABI::ValueEncoder::int256FromUint256(~uint256_t(1)), -2);
}

TEST(EthereumAbiValueEncoder, uint256FromInt256) {
    Data data;
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(0), 0);
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(1), 1);
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(100), 100);
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(-1), (~uint256_t(0)));
    EXPECT_EQ(ABI::ValueEncoder::uint256FromInt256(-2), (~uint256_t(1)));
}

TEST(EthereumAbiValueEncoder, pad32) {
    EXPECT_EQ(64ul, ABI::ValueEncoder::paddedTo32(40));
    EXPECT_EQ(32ul, ABI::ValueEncoder::paddedTo32(32));
    EXPECT_EQ(64ul, ABI::ValueEncoder::paddedTo32(33));
    EXPECT_EQ(64ul, ABI::ValueEncoder::paddedTo32(63));
    EXPECT_EQ(64ul, ABI::ValueEncoder::paddedTo32(64));
    EXPECT_EQ(96ul, ABI::ValueEncoder::paddedTo32(65));
    EXPECT_EQ(24ul, ABI::ValueEncoder::padNeeded32(40));
    EXPECT_EQ(0ul, ABI::ValueEncoder::padNeeded32(32));
    EXPECT_EQ(31ul, ABI::ValueEncoder::padNeeded32(33));
    EXPECT_EQ(1ul, ABI::ValueEncoder::padNeeded32(63));
    EXPECT_EQ(0ul, ABI::ValueEncoder::padNeeded32(64));
    EXPECT_EQ(31ul, ABI::ValueEncoder::padNeeded32(65));
}

} // namespace TW::Ethereum::tests
