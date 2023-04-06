// Copyright © 2017-2023 Trust Wallet.
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

namespace TW::Cosmos::tests {

TEST(TWAxelarAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidBech32(STRING("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5").get(), TWCoinTypeCosmos, STRING("axelar").get()));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5").get(), TWCoinTypeAxelar));
    EXPECT_FALSE(TWAnyAddressIsValidBech32(STRING("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5").get(), TWCoinTypeBitcoin, STRING("axelar").get()));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5").get(), TWCoinTypeCosmos));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5").get(), TWCoinTypeBitcoin));
}

TEST(TWAxelarAnyAddress, createFromPubKeyAxelar) {
    const auto hrp = STRING("axelar");
    const auto data = DATA("02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5");
    const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeSECP256k1);
    const auto twAddress = TWAnyAddressCreateBech32WithPublicKey(pubkey, TWCoinTypeCosmos, hrp.get());
    auto twData = TWAnyAddressData(twAddress);
    auto hexData = hex(*reinterpret_cast<const Data*>(twData));
    ASSERT_EQ(hexData, "d8c95f06c8ed16d8397fe11fb3af30f87bfb340f");
    ASSERT_EQ(hex(Bech32Address("axelar", TW::Hash::HasherSha256ripemd, pubkey->impl).getKeyHash()), hexData);
    auto address = TWAnyAddressDescription(twAddress);
    EXPECT_EQ("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5", *reinterpret_cast<const std::string*>(address));
    TWStringDelete(address);
    TWAnyAddressDelete(twAddress);
    TWDataDelete(twData);
    TWPublicKeyDelete(pubkey);
}

TEST(TWAxelarAnyAddress, createFromStringAxelar) {
    const auto AxelarAddress = STRING("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5");
    const auto hrp = STRING("axelar");
    const auto anyAddr = TWAnyAddressCreateBech32(AxelarAddress.get(), TWCoinTypeCosmos, hrp.get());
    const auto addrDescription = TWAnyAddressDescription(anyAddr);
    ASSERT_TRUE(TWAnyAddressIsValidBech32(addrDescription, TWCoinTypeCosmos, hrp.get()));
    TWStringDelete(addrDescription);
    TWAnyAddressDelete(anyAddr);
}

}
