// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>
#include "Hash.h"
#include "PublicKey.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {

inline constexpr uint32_t acalaPrefix{10};

TEST(TWAcalaAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidSS58(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypePolkadot, acalaPrefix));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypeAcala));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypePolkadot));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypeBitcoin));
    EXPECT_FALSE(TWAnyAddressIsValidSS58(STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get(), TWCoinTypePolkadot, acalaPrefix));
}

TEST(TWAcalaAnyAddress, createFromPubKey) {
    const auto data = DATA("e9590e4d99264a14a85e21e69537e4a64f66a875d38cb8f76b305f41fabe24a9");
    const auto pubkey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeED25519));
    const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubkey.get(), TWCoinTypeAcala));
    const auto addrDescription = TWAnyAddressDescription(addr.get());
    EXPECT_EQ("269ZCS3WLGydTN8ynhyhZfzJrXkePUcdhwgLQs6TWFs5wVL5", *reinterpret_cast<const std::string*>(addrDescription));
    TWStringDelete(addrDescription);
}

TEST(TWAcalaAnyAddress, createFromString) {
    const auto acalaAddress = STRING("24CKv1LJ1T3U9ujCN63YzTPuQjcmURGA2xTjim98UKXxgNXT");
    const auto anyAddr = TWAnyAddressCreateWithString(acalaAddress.get(), TWCoinTypeAcala);
    const auto addrDescription = TWAnyAddressDescription(anyAddr);
    ASSERT_TRUE(TWAnyAddressIsValidSS58(addrDescription, TWCoinTypePolkadot, acalaPrefix));
    ASSERT_TRUE(TWAnyAddressIsValid(addrDescription, TWCoinTypeAcala));
    TWStringDelete(addrDescription);
    TWAnyAddressDelete(anyAddr);
}

TEST(TWAcalaAnyAddress, createFromPubKeyAcalaPrefix) {
    const auto data = DATA("92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");
    const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeED25519);
    const auto twAddress = TWAnyAddressCreateSS58WithPublicKey(pubkey, TWCoinTypePolkadot, acalaPrefix);
    auto address = TWAnyAddressDescription(twAddress);
    EXPECT_EQ("24CKv1LJ1T3U9ujCN63YzTPuQjcmURGA2xTjim98UKXxgNXT", *reinterpret_cast<const std::string*>(address));
    TWStringDelete(address);
    TWAnyAddressDelete(twAddress);
    TWPublicKeyDelete(pubkey);
}

TEST(TWAcalaAnyAddress, createFromStringAcalaPrefix) {
    const auto acalaAddress = STRING("24CKv1LJ1T3U9ujCN63YzTPuQjcmURGA2xTjim98UKXxgNXT");
    const auto anyAddr = TWAnyAddressCreateSS58(acalaAddress.get(), TWCoinTypePolkadot, acalaPrefix);
    const auto addrDescription = TWAnyAddressDescription(anyAddr);
    ASSERT_TRUE(TWAnyAddressIsValidSS58(addrDescription, TWCoinTypePolkadot, acalaPrefix));
    TWStringDelete(addrDescription);
    TWAnyAddressDelete(anyAddr);
}

}
