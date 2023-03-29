// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Address.h"
#include "Bitcoin/Script.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>
#include <string>

using namespace TW;

namespace TW::Bitcoin::tests {

const char* TestPubKey1 = "039d645d2ce630c2a9a6dbe0cbd0a8fcb7b70241cb8a48424f25593290af2494b9";
const char* TestP2phkAddr1 = "12dNaXQtN5Asn2YFwT1cvciCrJa525fAe4";
const char* TestP2phkData1 = "0011d91ce1cc681f95583da3f4a6841c174be950c7";
const char* TestP2shAddr1 = "3PQ5BD39rDikf7YW6pJ9a9tbS3QhvwvzTG";
const char* TestP2shData1 = "05ee1e69460b59027d9df0a79ca2c92aa382a25fb7";

TEST(BitcoinAddress, P2PKH_CreateFromString) {
    const auto address = Address(TestP2phkAddr1);
    EXPECT_EQ(address.string(), TestP2phkAddr1);
    EXPECT_EQ(hex(address.bytes), TestP2phkData1);
}

TEST(BitcoinAddress, P2PKH_CreateFromPubkey) {
    const auto publicKey = PublicKey(parse_hex(TestPubKey1), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeBitcoin));
    EXPECT_EQ(address.string(), TestP2phkAddr1);
    EXPECT_EQ(hex(address.bytes), TestP2phkData1);
}

TEST(BitcoinAddress, P2PKH_CreateFromData) {
    const auto address = Address(parse_hex(TestP2phkData1));
    EXPECT_EQ(address.string(), TestP2phkAddr1);
    EXPECT_EQ(hex(address.bytes), TestP2phkData1);
}

TEST(BitcoinAddress, P2SH_CreateFromString) {
    const auto address = Address(TestP2shAddr1);
    EXPECT_EQ(address.string(), TestP2shAddr1);
    EXPECT_EQ(hex(address.bytes), TestP2shData1);
}

TEST(BitcoinAddress, P2WPKH_Nested_P2SH) {
    // P2SH address cannot be created directly from pubkey, script is built
    const auto publicKey = PublicKey(parse_hex(TestPubKey1), TWPublicKeyTypeSECP256k1);

    const auto pubKeyHash = publicKey.hash({});
    EXPECT_EQ(hex(pubKeyHash), "11d91ce1cc681f95583da3f4a6841c174be950c7");

    const auto script = Script::buildPayToV0WitnessProgram(pubKeyHash);
    EXPECT_EQ(hex(script.bytes), "0014"
                                 "11d91ce1cc681f95583da3f4a6841c174be950c7");

    const auto scriptHash = Hash::sha256ripemd(script.bytes.data(), script.bytes.size());
    EXPECT_EQ(hex(scriptHash), "ee1e69460b59027d9df0a79ca2c92aa382a25fb7");

    Data addressData = {TWCoinTypeP2shPrefix(TWCoinTypeBitcoin)};
    TW::append(addressData, scriptHash);
    EXPECT_EQ(hex(addressData), TestP2shData1);

    const auto address = Address(addressData);
    EXPECT_EQ(address.string(), TestP2shAddr1);
    EXPECT_EQ(hex(address.bytes), TestP2shData1);
}

TEST(BitcoinAddress, P2SH_CreateFromData) {
    const auto address = Address(parse_hex(TestP2shData1));
    EXPECT_EQ(address.string(), TestP2shAddr1);
    EXPECT_EQ(hex(address.bytes), TestP2shData1);
}

} // namespace TW::Bitcoin::tests
