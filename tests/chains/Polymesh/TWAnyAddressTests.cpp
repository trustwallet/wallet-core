// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;

namespace TW::Polymesh::tests {
extern uint32_t polymeshPrefix;

TEST(TWPolymesh, Address) {
    auto string = STRING("2E5u4xA1TqswQ3jMJH96zekxwr2itvKu79fDC1mmnVZRh6Uv");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypePolymesh));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867");
}

TEST(PolymeshAddress, Validation) {
    // Substrate ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ").get(), TWCoinTypePolymesh));
    // Bitcoin
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("1ES14c7qLb5CYhLMUekctxLgc1FV2Ti9DA").get(), TWCoinTypePolymesh));
    // Kusama ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("FHKAe66mnbk8ke8zVWE9hFVFrJN1mprFPVmD5rrevotkcDZ").get(), TWCoinTypePolymesh));
    // Kusama secp256k1
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("FxQFyTorsjVsjjMyjdgq8w5vGx8LiA1qhWbRYcFijxKKchx").get(), TWCoinTypePolymesh));
    // Kusama sr25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("EJ5UJ12GShfh7EWrcNZFLiYU79oogdtXFUuDDZzk7Wb2vCe").get(), TWCoinTypePolymesh));

    // Polkadot ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get(), TWCoinTypePolymesh));
    // Polkadot sr25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony").get(), TWCoinTypePolymesh));

    // Polymesh
    ASSERT_TRUE(TWAnyAddressIsValid(STRING("2DxwekgWwK7sqVeuXGmaXLZUvwnewLTs2rvU2CFKLgvvYwCG").get(), TWCoinTypePolymesh));
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A").get(), TWCoinTypePolymesh));
    ASSERT_TRUE(TWAnyAddressIsValidSS58(STRING("2DxwekgWwK7sqVeuXGmaXLZUvwnewLTs2rvU2CFKLgvvYwCG").get(), TWCoinTypePolymesh, polymeshPrefix));
    ASSERT_FALSE(TWAnyAddressIsValidSS58(STRING("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A").get(), TWCoinTypePolymesh, polymeshPrefix));
}

TEST(PolymeshAddress, FromPrivateKey) {
    // subkey phrase `chief menu kingdom stereo hope hazard into island bag trick egg route`
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("0x612d82bc053d1b4729057688ecb1ebf62745d817ddd9b595bc822f5f2ba0e41a").get()));
    const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypePolymesh));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypePolymesh));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), STRING("2GmLy7KywpsV5fDpZfJMcgGgzoJWyrEA3Wc3fDmsoq5iqtBT").get()));
}

TEST(PolymeshAddress, FromPublicKey) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("0xbeff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908").get(), TWPublicKeyTypeED25519));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypePolymesh));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), STRING("2GmLy7KywpsV5fDpZfJMcgGgzoJWyrEA3Wc3fDmsoq5iqtBT").get()));
}

TEST(PolymeshAddress, FromPublicKeyWithPrefix) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f83").get(), TWPublicKeyTypeED25519));
    const auto addressPolymesh = STRING("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58WithPublicKey(publicKey.get(), TWCoinTypePolymesh, polymeshPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), addressPolymesh.get()));
    }
}

TEST(PolymeshAddress, FromString) {
    auto string = STRING("2E5u4xA1TqswQ3jMJH96zekxwr2itvKu79fDC1mmnVZRh6Uv");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypePolymesh));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867");
}

TEST(PolymeshAddress, FromStringWithPrefix) {
    // polymesh
    auto addressPolymesh = STRING("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58(addressPolymesh.get(), TWCoinTypePolymesh, polymeshPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), addressPolymesh.get()));
    }
}

} // namespace TW::Polymesh::tests