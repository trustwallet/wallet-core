// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWJunoAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidWithHrp(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeCosmos, STRING("juno").get()));
    EXPECT_FALSE(TWAnyAddressIsValidWithHrp(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeBitcoin, STRING("juno").get()));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeCosmos));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeBitcoin));

}

TEST(TWJunoAnyAddress, createFromPubKeyJuno) {
    const auto data = DATA("02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc");
    const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeSECP256k1);
    const auto twAddress = TWAnyAddressCreateWithPublicKeyAndHrp(pubkey, TWCoinTypeCosmos, STRING("juno").get());
    TWPublicKeyDelete(pubkey);
    auto address = TWAnyAddressDescription(twAddress);
    EXPECT_EQ("juno1cj2vfjec3c3luf9fx9vddnglhh9gawmncn4k5n", *reinterpret_cast<const std::string*>(address));
    TWStringDelete(address);
    TWAnyAddressDelete(twAddress);
}

TEST(TWJunoAnyAddress, createFromStringJuno) {
    const auto junoAddress = STRING("juno1cj2vfjec3c3luf9fx9vddnglhh9gawmncn4k5n");
    const auto hrp = STRING("juno");
    const auto anyAddr = TWAnyAddressCreateWithStringAndHrp(junoAddress.get(), TWCoinTypeCosmos, hrp.get());
    const auto addrDescription = TWAnyAddressDescription(anyAddr);
    ASSERT_TRUE(TWAnyAddressIsValidWithHrp(addrDescription, TWCoinTypeCosmos, hrp.get()));
    TWStringDelete(addrDescription);
    TWAnyAddressDelete(anyAddr);
}
