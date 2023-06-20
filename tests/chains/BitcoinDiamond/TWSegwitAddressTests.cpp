// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "TestUtilities.h"
#include "Bitcoin/SegwitAddress.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"
#include <gtest/gtest.h>
#include <TrustWalletCore/TWAnyAddress.h>

using namespace TW;

TEST(TWBitcoinDiamondSegwitAddress, Valid) {
    ASSERT_TRUE(Bitcoin::SegwitAddress::isValid("bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5wh"));
    ASSERT_FALSE(Bitcoin::SegwitAddress::isValid("bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5ac"));
}

/// Initializes a Bech32 address with a human-readable part, a witness
/// version, and a witness program.
TEST(TWBitcoinDiamondSegwitAddress, WitnessProgramToAddress) {
    auto address = Bitcoin::SegwitAddress("bcd", 0, parse_hex("8d17543f223171fa005dc557f8fbd32d4aae0960"));

    ASSERT_TRUE(Bitcoin::SegwitAddress::isValid(address.string()));
    ASSERT_EQ(address.string(), "bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5wh");
}

/// Initializes a Bech32 address with a public key and a HRP prefix.
TEST(TWBitcoinDiamondSegwitAddress, PubkeyToAddress) {
    const auto publicKey = PublicKey(parse_hex("032a9ccb9cc6fd461df091b0f711730daa4292f9226aec918ac19381ac2af5e9ee"), TWPublicKeyTypeSECP256k1);

    /// construct with public key
    auto address = Bitcoin::SegwitAddress(publicKey, "bcd");

    ASSERT_TRUE(Bitcoin::SegwitAddress::isValid(address.string()));
    ASSERT_EQ(address.string(), "bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5wh");
}

/// Decodes a SegWit address.
TEST(TWBitcoinDiamondSegwitAddress, Decode) {
    auto result = Bitcoin::SegwitAddress::decode("bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5wh");

    ASSERT_TRUE(std::get<2>(result));
    ASSERT_EQ(std::get<0>(result).string(), "bcd1q35t4g0ezx9cl5qzac4tl377n9492uztqjwf5wh");
    ASSERT_EQ(std::get<1>(result), "bcd");
}
