// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>
#include "PublicKey.h"
#include "Bech32Address.h"
#include "HexCoding.h"
#include "Hash.h"
#include "PublicKey.h"
#include "Bech32Address.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWJunoAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidBech32(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeCosmos, STRING("juno").get()));
    EXPECT_FALSE(TWAnyAddressIsValidBech32(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeBitcoin, STRING("juno").get()));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeCosmos));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("juno1gckvjxau7k56f8wg8c8xj80khyp83y8x8eqc94").get(), TWCoinTypeBitcoin));

}

TEST(TWJunoAnyAddress, createFromPubKeyJuno) {
    const auto hrp = STRING("juno");
    const auto data = DATA("02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc");
    const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeSECP256k1);
    const auto twAddress = TWAnyAddressCreateBech32WithPublicKey(pubkey, TWCoinTypeCosmos, hrp.get());
    auto twData = TWAnyAddressData(twAddress);
    auto hexData = hex(*reinterpret_cast<const Data*>(twData));
    ASSERT_EQ(hexData, "c494c4cb388e23fe24a93158d6cd1fbdca8ebb73");
    ASSERT_EQ(hex(Bech32Address("juno", TW::Hash::HasherSha256ripemd, pubkey->impl).getKeyHash()), hexData);
    auto address = TWAnyAddressDescription(twAddress);
    EXPECT_EQ("juno1cj2vfjec3c3luf9fx9vddnglhh9gawmncn4k5n", *reinterpret_cast<const std::string*>(address));
    TWStringDelete(address);
    TWAnyAddressDelete(twAddress);
    TWDataDelete(twData);
    TWPublicKeyDelete(pubkey);
}

TEST(TWJunoAnyAddress, createFromStringJuno) {
    const auto junoAddress = STRING("juno1cj2vfjec3c3luf9fx9vddnglhh9gawmncn4k5n");
    const auto hrp = STRING("juno");
    const auto anyAddr = TWAnyAddressCreateBech32(junoAddress.get(), TWCoinTypeCosmos, hrp.get());
    const auto addrDescription = TWAnyAddressDescription(anyAddr);
    ASSERT_TRUE(TWAnyAddressIsValidBech32(addrDescription, TWCoinTypeCosmos, hrp.get()));
    TWStringDelete(addrDescription);
    TWAnyAddressDelete(anyAddr);

}
