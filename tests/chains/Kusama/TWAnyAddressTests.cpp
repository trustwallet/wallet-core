// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>
#include <vector>

namespace TW::Kusama::tests {

TEST(KusamaAddress, Validation) {
    // Substrate ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ").get(), TWCoinTypeKusama));
    // Polkadot ed25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony").get(), TWCoinTypeKusama));
    // Polkadot sr25519
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony").get(), TWCoinTypeKusama));
    // Bitcoin
    ASSERT_FALSE(TWAnyAddressIsValid(STRING("1ES14c7qLb5CYhLMUekctxLgc1FV2Ti9DA").get(), TWCoinTypeKusama));

    // Kusama ed25519
    ASSERT_TRUE(TWAnyAddressIsValid(STRING("FHKAe66mnbk8ke8zVWE9hFVFrJN1mprFPVmD5rrevotkcDZ").get(), TWCoinTypeKusama));
    // Kusama secp256k1
    ASSERT_TRUE(TWAnyAddressIsValid(STRING("FxQFyTorsjVsjjMyjdgq8w5vGx8LiA1qhWbRYcFijxKKchx").get(), TWCoinTypeKusama));
    // Kusama sr25519
    ASSERT_TRUE(TWAnyAddressIsValid(STRING("EJ5UJ12GShfh7EWrcNZFLiYU79oogdtXFUuDDZzk7Wb2vCe").get(), TWCoinTypeKusama));
}

TEST(KusamaAddress, FromPrivateKey) {
    // from subkey: tiny escape drive pupil flavor endless love walk gadget match filter luxury
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("0xa21981f3bb990c40837df44df639541ff57c5e600f9eb4ac00ed8d1f718364e5").get()));
    const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKey(privateKey.get(), TWCoinTypeKusama));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeKusama));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), STRING("CeVXtoU4py9e7F6upfM2ZarVave299TjcdaTSxhDDZrYgnM").get()));
}

TEST(KusamaAddress, FromPublicKey) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA("0x032eb287017c5cde2940b5dd062d413f9d09f8aa44723fc80bf46b96c81ac23d").get(), TWPublicKeyTypeED25519));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeKusama));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), STRING("CeVXtoU4py9e7F6upfM2ZarVave299TjcdaTSxhDDZrYgnM").get()));
}

TEST(KusamaAddress, FromString) {
    auto addressStr1 = STRING("CeVXtoU4py9e7F6upfM2ZarVave299TjcdaTSxhDDZrYgnM");
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(addressStr1.get(), TWCoinTypeKusama));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
    EXPECT_TRUE(TWStringEqual(addressStr.get(), addressStr1.get()));
}

} // namespace TW::Kusama::tests
