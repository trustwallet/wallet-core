// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>
#include "HexCoding.h"
#include "Hash.h"
#include "PublicKey.h"
#include "Bech32Address.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {

inline constexpr uint32_t acalaPrefix{10};

TEST(TWAcalaAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidSS58(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypePolkadot, acalaPrefix));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypePolkadot));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypeBitcoin));
    EXPECT_FALSE(TWAnyAddressIsValidSS58(STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get(), TWCoinTypePolkadot, acalaPrefix));
}

TEST(TWAcalaAnyAddress, createFromPubKeyAcala) {
    const auto data = DATA("92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3");
    const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeED25519);
    const auto twAddress = TWAnyAddressCreateSS58WithPublicKey(pubkey, TWCoinTypePolkadot, acalaPrefix);
    auto address = TWAnyAddressDescription(twAddress);
    EXPECT_EQ("24CKv1LJ1T3U9ujCN63YzTPuQjcmURGA2xTjim98UKXxgNXT", *reinterpret_cast<const std::string*>(address));
    TWStringDelete(address);
    TWAnyAddressDelete(twAddress);
    TWPublicKeyDelete(pubkey);
}

TEST(TWAcalaAnyAddress, createFromStringAcala) {
    const auto acalaAddress = STRING("24CKv1LJ1T3U9ujCN63YzTPuQjcmURGA2xTjim98UKXxgNXT");
    const auto anyAddr = TWAnyAddressCreateSS58(acalaAddress.get(), TWCoinTypePolkadot, acalaPrefix);
    const auto addrDescription = TWAnyAddressDescription(anyAddr);
    ASSERT_TRUE(TWAnyAddressIsValidSS58(addrDescription, TWCoinTypePolkadot, acalaPrefix));
    TWStringDelete(addrDescription);
    TWAnyAddressDelete(anyAddr);

}

}
