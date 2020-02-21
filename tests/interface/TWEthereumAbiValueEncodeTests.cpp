// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValueEncode.h>

#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"
#include "TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace std;

const string brownFox = "The quick brown fox jumps over the lazy dog";
const string brownFoxDot = "The quick brown fox jumps over the lazy dog.";
const string hello = "hello";

TEST(TWEthereumAbiValueEncode, encodeBool) {
    auto data = TWDataCreateWithSize(32);

    TWEthereumAbiValueEncodeBool(false, data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000000000000000000000000000000000000000000000");
    TWEthereumAbiValueEncodeBool(true, data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000000000000000000000000000000000000000000001");

    TWDataDelete(data);
}

TWData* _Nonnull buildUInt256(const uint256_t& value) {
    const Data data = TW::store(value);
    return TWDataCreateWithBytes(data.data(), data.size());
}

TEST(TWEthereumAbiValueEncode, encodeInt) {
    auto data = TWDataCreateWithSize(32);

    TWEthereumAbiValueEncodeInt32(69, data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000000000000000000000000000000000000000000045");
    TWEthereumAbiValueEncodeInt32(-1, data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    TWEthereumAbiValueEncodeUInt32(69, data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000000000000000000000000000000000000000000045");
    TWEthereumAbiValueEncodeUInt256(buildUInt256(uint256_t(69)), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000000000000000000000000000000000000000000045");
    TWEthereumAbiValueEncodeInt256(buildUInt256(uint256_t(69)), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000000000000000000000000000000000000000000045");
    TWEthereumAbiValueEncodeInt256(buildUInt256(~uint256_t(0)), data); // int256_t(-1)
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    TWDataDelete(data);
}

TEST(TWEthereumAbiValueEncode, encodeAddress) {
    auto data = TWDataCreateWithSize(32);
    TWEthereumAbiValueEncodeAddress(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "0000000000000000000000005aaeb6053f3e94c9b9a09f33669435e7ef1beaed");
    TWDataDelete(data);
}

TEST(TWEthereumAbiValueEncode, encodeString) {
    auto data = TWDataCreateWithSize(32);
    TWEthereumAbiValueEncodeString(TWStringCreateWithUTF8Bytes("trustwallet"), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
    TWDataDelete(data);
}

TEST(TWEthereumAbiValueEncode, encodeBytes) {
    auto data = TWDataCreateWithSize(32);

    TWEthereumAbiValueEncodeBytes(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("45")), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "4500000000000000000000000000000000000000000000000000000000000000");
    TWEthereumAbiValueEncodeBytes(TWDataCreateWithHexString(TWStringCreateWithUTF8Bytes("5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "5aaeb6053f3e94c9b9a09f33669435e7ef1beaed000000000000000000000000");
    
    TWDataDelete(data);
}

TEST(TWEthereumAbiValueEncode, encodeBytesDyn) {
    auto data = TWDataCreateWithSize(32);
    std::string valueStr = "trustwallet";
    TWEthereumAbiValueEncodeBytesDyn(TWDataCreateWithBytes(reinterpret_cast<const unsigned char*>(valueStr.c_str()), valueStr.length()), data);
    EXPECT_EQ(hex(*reinterpret_cast<const Data*>(data)), "31924c4e2bb082322d1efa718bf67c73ca297b481dac9f76ad35670cff0056a3");
    TWDataDelete(data);
}
