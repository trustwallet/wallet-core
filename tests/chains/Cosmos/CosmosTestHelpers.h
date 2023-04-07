// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include "HexCoding.h"
#include "Hash.h"
#include "PublicKey.h"
#include "Bech32Address.h"
#include "Cosmos/Address.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests::internal {
    static constexpr const char* gPubKey = "02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5";
    static constexpr const char* gPrivKey = "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433";

    static inline void isValidAddressWrapper(const std::string& hrp, TWCoinType coinType, const std::string& address) {
        auto address_utf8 = STRING(address.c_str());
        auto hrp_utf8 = STRING(hrp.c_str());
        EXPECT_TRUE(TWAnyAddressIsValidBech32(address_utf8.get(), TWCoinTypeCosmos, hrp_utf8.get()));
        EXPECT_TRUE(TWAnyAddressIsValid(address_utf8.get(), coinType));
        EXPECT_FALSE(TWAnyAddressIsValidBech32(address_utf8.get(), TWCoinTypeBitcoin, hrp_utf8.get()));
        EXPECT_FALSE(TWAnyAddressIsValid(address_utf8.get(), TWCoinTypeCosmos));
        EXPECT_FALSE(TWAnyAddressIsValid(address_utf8.get(), TWCoinTypeBitcoin));
    }

    static inline void createFromPubKeyWrapper(const std::string& hrp, TWCoinType coinType, const std::string& address_str, TWPublicKeyType pubKeyType = TWPublicKeyTypeSECP256k1) {
        // BECH32
        {
            const auto hrp_utf8 = STRING(hrp.c_str());
            const auto data = DATA(gPubKey);
            const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeSECP256k1);
            const auto twAddress = TWAnyAddressCreateBech32WithPublicKey(pubkey, TWCoinTypeCosmos, hrp_utf8.get());
            auto twData = TWAnyAddressData(twAddress);
            auto hexData = hex(*reinterpret_cast<const Data*>(twData));
            ASSERT_EQ(hex(Bech32Address(hrp, TW::Hash::HasherSha256ripemd, pubkey->impl).getKeyHash()), hexData);
            auto address = TWAnyAddressDescription(twAddress);
            EXPECT_EQ(address_str, *reinterpret_cast<const std::string*>(address));
            TWStringDelete(address);
            TWAnyAddressDelete(twAddress);
            TWDataDelete(twData);
            TWPublicKeyDelete(pubkey);
        }

        // With coin type
        {
            auto publicKey = PublicKey(parse_hex(gPubKey), pubKeyType);
            auto address = Address(coinType, publicKey);
        }
    }

    static inline void createFromPrivKeyWrapper(TWCoinType coinType, const std::string& address_str, TWPublicKeyType pubKeyType = TWPublicKeyTypeSECP256k1) {
        auto privateKey = PrivateKey(parse_hex(gPrivKey));
        auto address = Address(coinType, privateKey.getPublicKey(pubKeyType));
        ASSERT_EQ(address.string(), address_str);
    }

    static inline void createFromStringWrapper(const std::string& hrp_str, TWCoinType coinType, const std::string& address_str) {
        // BECH32
        {
            const auto address = STRING(address_str.c_str());
            const auto hrp = STRING(hrp_str.c_str());
            const auto anyAddr = TWAnyAddressCreateBech32(address.get(), TWCoinTypeCosmos, hrp.get());
            const auto addrDescription = TWAnyAddressDescription(anyAddr);
            ASSERT_TRUE(TWAnyAddressIsValidBech32(addrDescription, TWCoinTypeCosmos, hrp.get()));
            TWStringDelete(addrDescription);
            TWAnyAddressDelete(anyAddr);
        }

        // With Coin Type
        {
            const auto address = STRING(address_str.c_str());
            const auto anyAddr = TWAnyAddressCreateWithString(address.get(), coinType);
            const auto addrDescription = TWAnyAddressDescription(anyAddr);
            ASSERT_TRUE(TWAnyAddressIsValid(addrDescription, coinType));
            TWStringDelete(addrDescription);
            TWAnyAddressDelete(anyAddr);
        }
    }
}

namespace TW::Cosmos::tests {
    static inline void allAddressTestsWrapper(const std::string& hrp_str, TWCoinType coinType, const std::string& address_str) {
        internal::isValidAddressWrapper(hrp_str, coinType, address_str);
        internal::createFromPubKeyWrapper(hrp_str, coinType, address_str);
        internal::createFromPrivKeyWrapper(coinType, address_str);
        internal::createFromStringWrapper(hrp_str, coinType, address_str);
    }
}
