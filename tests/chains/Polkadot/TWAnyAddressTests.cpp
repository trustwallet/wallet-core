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

namespace TW::Polkadot::tests {
extern uint32_t polkadotPrefix;
extern uint32_t kusamaPrefix;
extern uint32_t astarPrefix;
extern uint32_t parallelPrefix;

TEST(PolkadotAddress, Validation) {
    // Substrate ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ").get(), TWCoinTypePolkadot));
    // Bitcoin
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("1ES14c7qLb5CYhLMUekctxLgc1FV2Ti9DA").get(), TWCoinTypePolkadot));
    // Kusama ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("FHKAe66mnbk8ke8zVWE9hFVFrJN1mprFPVmD5rrevotkcDZ").get(), TWCoinTypePolkadot));
    // Kusama secp256k1
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("FxQFyTorsjVsjjMyjdgq8w5vGx8LiA1qhWbRYcFijxKKchx").get(), TWCoinTypePolkadot));
    // Kusama sr25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("EJ5UJ12GShfh7EWrcNZFLiYU79oogdtXFUuDDZzk7Wb2vCe").get(), TWCoinTypePolkadot));

    // Polkadot ed25519
    ASSERT_TRUE(TWAnyAddressIsValid(STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get(), TWCoinTypePolkadot));
    // Polkadot sr25519
    ASSERT_TRUE(TWAnyAddressIsValid(STRING("15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony").get(), TWCoinTypePolkadot));

    ASSERT_TRUE(TWAnyAddressIsValidSS58(STRING("cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb").get(), TWCoinTypePolkadot, 64));
    ASSERT_FALSE(TWAnyAddressIsValidSS58(STRING("JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A").get(), TWCoinTypePolkadot, 64));
}

TEST(PolkadotAddress, FromPrivateKey) {
    // subkey phrase `chief menu kingdom stereo hope hazard into island bag trick egg route`
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("0x612d82bc053d1b4729057688ecb1ebf62745d817ddd9b595bc822f5f2ba0e41a").get()));
    const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypePolkadot));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypePolkadot));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get()));
}

TEST(PolkadotAddress, FromPublicKey) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("0xbeff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908").get(), TWPublicKeyTypeED25519));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypePolkadot));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get()));
}

TEST(PolkadotAddress, FromPublicKeyWithPrefix) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3").get(), TWPublicKeyTypeED25519));

    const auto addressPolkadot = STRING("14KjL5vGAYJCbKgZJmFKDSjewtBpvaxx9YvRZvi7qmb5s8CC");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58WithPublicKey(publicKey.get(), TWCoinTypePolkadot, polkadotPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), addressPolkadot.get()));
    }

    const auto addressAstar = STRING("ZG2d3dH5zfqNchsqReS6x4nBJuJCW7Z6Fh5eLvdA3ZXGkPd");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58WithPublicKey(publicKey.get(), TWCoinTypePolkadot, astarPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), addressAstar.get()));
    }

    const auto addressParallel = STRING("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58WithPublicKey(publicKey.get(), TWCoinTypePolkadot, parallelPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), addressParallel.get()));
    }
}

TEST(PolkadotAddress, FromString) {
    auto addressStr1 = STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(addressStr1.get(), TWCoinTypePolkadot));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), addressStr1.get()));
}

TEST(PolkadotAddress, FromStringWithPrefix) {
    const auto kusamaAddress = STRING("Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58(kusamaAddress.get(), TWCoinTypeKusama, kusamaPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), kusamaAddress.get()));
    }

    auto addressParallel = STRING("p8EGHjWt7e1MYoD7V6WXvbPZWK9GSJiiK85kv2R7Ur7FisPUL");
    {
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateSS58(addressParallel.get(), TWCoinTypePolkadot, parallelPrefix));
        const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        EXPECT_TRUE(TWStringEqual(addressStr.get(), addressParallel.get()));
    }
}

} // namespace TW::Polkadot::tests
