// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Harmony/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Harmony;

// EthereumAddress format == Harmony Address format: 20 bytes

TEST(HarmonyAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
    ASSERT_FALSE(Address::isValid("fB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"));
}

TEST(HarmonyAddress, EIP55) {
    
    // Harmony address is Bech32 after obtaining Ethereum ... hasing
    // TBD
    
    ASSERT_EQ(
        Address(parse_hex("5aaeb6053f3e94c9b9a09f33669435e7ef1beaed")).string(),
        "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"
    );
    ASSERT_EQ(
        Address(parse_hex("0x5AAEB6053F3E94C9b9A09f33669435E7Ef1BEAED")).string(),
        "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"
    );
    ASSERT_EQ(
        Address(parse_hex("0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359")).string(),
        "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"
    );
    ASSERT_EQ(
        Address(parse_hex("0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB")).string(),
        "0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB"
    );
    ASSERT_EQ(
        Address(parse_hex("0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb")).string(),
        "0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb"
    );
}

TEST(HarmonyAddress, String) {
    const auto address = Address("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed");
    ASSERT_EQ(address.string(), "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed");
}

// Harmony address is Bech32 after obtaining Ethereum ... hasing
// TBD
// double check: ... Extended

TEST(HarmonyAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309");
}


// EthereumAddress format == Harmony Address format: 20 bytes

//TEST(HarmonyAddress, IsValid) {
//    ASSERT_FALSE(Address::isValid("abc"));
//    ASSERT_TRUE(Address::isValid("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"));
//}

TEST(HarmonyAddress, IsValid) {
    ASSERT_FALSE(Address::isValid("abc"));
    //YZ test the Harmony address
    ASSERT_TRUE(Address::isValid("0xD7Ff41CA29306122185A07d04293DdB35F24Cf2d"));
    ASSERT_TRUE(Address::isValid("0x806171f95c5a74371a19e8a312c9e5cb4e1d24f6"));
    ASSERT_TRUE(Address::isValid("0xd7ff41ca29306122185a07d04293ddb35f24cf2d"));
}


