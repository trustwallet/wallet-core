// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValue.h>

#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace std;

const string brownFox = "The quick brown fox jumps over the lazy dog";
const string brownFoxDot = "The quick brown fox jumps over the lazy dog.";
const string hello = "hello";

TEST(TWEthereumAbiValue, encodeBool) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeBool(false)).get())),
        "0000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeBool(true)).get())),
        "0000000000000000000000000000000000000000000000000000000000000001");
}

TWData* _Nonnull buildUInt256(const uint256_t& value) {
    const Data data = TW::store(value);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TEST(TWEthereumAbiValue, encodeInt) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeInt32(69)).get())), 
        "0000000000000000000000000000000000000000000000000000000000000045");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeInt32(-1)).get())),
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeUInt32(69)).get())),
        "0000000000000000000000000000000000000000000000000000000000000045");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeUInt256(WRAPD(buildUInt256(uint256_t(69))).get())).get())),
        "0000000000000000000000000000000000000000000000000000000000000045");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeInt256(WRAPD(buildUInt256(uint256_t(69))).get())).get())),
        "0000000000000000000000000000000000000000000000000000000000000045");
    // int256(-1)
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeInt256(WRAPD(buildUInt256(~uint256_t(0))).get())).get())),
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
}

TEST(TWEthereumAbiValue, encodeAddress) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeAddress(WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")).get())).get())).get())),
        "0000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed");
}

TEST(TWEthereumAbiValue, encodeString) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeString(WRAPS(TWStringCreateWithUTF8Bytes("trustwallet")).get())).get())),
        "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}

TEST(TWEthereumAbiValue, encodeBytes) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeBytes(WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("45")).get())).get())).get())),
        "4500000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeBytes(WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes("5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")).get())).get())).get())),
        "5aaeb6053f3e94c9b9a09f33669435e7ef1beaed000000000000000000000000");
}

TEST(TWEthereumAbiValue, encodeBytesDyn) {
    std::string valueStr = "trustwallet";
    EXPECT_EQ(
        hex(*reinterpret_cast<const Data*>(WRAPD(TWEthereumAbiValueEncodeBytesDyn(WRAPD(
            TWDataCreateWithBytes(reinterpret_cast<const unsigned char*>(valueStr.c_str()), valueStr.length())
        ).get())).get())),
        "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3"
    );
}
