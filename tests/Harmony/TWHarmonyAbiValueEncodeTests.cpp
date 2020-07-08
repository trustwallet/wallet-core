// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonyAbiValueEncoder.h>

#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace std;

const string brownFox = "The quick brown fox jumps over the lazy dog";
const string brownFoxDot = "The quick brown fox jumps over the lazy dog.";
const string hello = "hello";

TEST(TWHarmonyAbiValueEncoder, encodeBool) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeBool(false)).get())),
        "0000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeBool(true)).get())),
        "0000000000000000000000000000000000000000000000000000000000000001");
}

static TWData* _Nonnull buildUInt256(const uint256_t& value) {
    const Data data = TW::store(value);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TEST(TWHarmonyAbiValueEncoder, encodeInt) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeInt32(69)).get())), 
        "0000000000000000000000000000000000000000000000000000000000000045");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeInt32(-1)).get())),
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeUInt32(69)).get())),
        "0000000000000000000000000000000000000000000000000000000000000045");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeUInt256(buildUInt256(uint256_t(69)))).get())),
        "0000000000000000000000000000000000000000000000000000000000000045");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeInt256(buildUInt256(uint256_t(69)))).get())),
        "0000000000000000000000000000000000000000000000000000000000000045");
    // int256(-1)
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeInt256(buildUInt256(~uint256_t(0)))).get())),
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
}

TEST(TWHarmonyAbiValueEncoder, encodeAddress) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeAddress(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")))).get())),
        "0000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed");
}

TEST(TWHarmonyAbiValueEncoder, encodeString) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeString(TWStringCreateWithUTF8Bytes("trustwallet"))).get())),
        "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}

TEST(TWHarmonyAbiValueEncoder, encodeBytes) {
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeBytes(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("45")))).get())),
        "4500000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeBytes(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")))).get())),
        "5aaeb6053f3e94c9b9a09f33669435e7ef1beaed000000000000000000000000");
}

TEST(TWHarmonyAbiValueEncoder, encodeBytesDyn) {
    std::string valueStr = "trustwallet";
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(WRAPD(TWHarmonyAbiValueEncoderEncodeBytesDyn(TWDataCreateWithBytes(reinterpret_cast<const unsigned char*>(valueStr.c_str()), valueStr.length()))).get())),
        "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
}
