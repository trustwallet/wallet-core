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

namespace TW::Cosmos::tests {

struct CosmosAddressParameters {
    const std::string hrp{};
    TWCoinType coinType;
    const std::string address;
    bool standaloneChain{true};
    TWPublicKeyType publicKeyType{TWPublicKeyTypeSECP256k1};
    const std::string privKey{"a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433"};
    const std::string publicKey{"02cbfdb5e472893322294e60cf0883d43df431e1089d29ecb447a9e6d55045aae5"};
};

}

namespace TW::Cosmos::tests::internal {
    static inline void isValidAddress(const CosmosAddressParameters& addressParameters) {
        auto address_utf8 = STRING(addressParameters.address.c_str());
        auto hrp_utf8 = STRING(addressParameters.hrp.c_str());
        EXPECT_TRUE(TWAnyAddressIsValidBech32(address_utf8.get(), TWCoinTypeCosmos, hrp_utf8.get()));
        EXPECT_TRUE(TWAnyAddressIsValid(address_utf8.get(), addressParameters.coinType));
        EXPECT_FALSE(TWAnyAddressIsValidBech32(address_utf8.get(), TWCoinTypeBitcoin, hrp_utf8.get()));
        if (addressParameters.coinType != TWCoinTypeCosmos) {
            EXPECT_FALSE(TWAnyAddressIsValid(address_utf8.get(), TWCoinTypeCosmos));
        }
        EXPECT_FALSE(TWAnyAddressIsValid(address_utf8.get(), TWCoinTypeBitcoin));
    }

    static inline void testCreateFromPubKeyWrapper(const CosmosAddressParameters& addressParameters) {
        if (addressParameters.standaloneChain) {
            const auto hrp_utf8 = STRING(addressParameters.hrp.c_str());
            const auto data = DATA(addressParameters.publicKey.c_str());
            const auto pubkey = TWPublicKeyCreateWithData(data.get(), TWPublicKeyTypeSECP256k1);
            const auto twAddress = TWAnyAddressCreateBech32WithPublicKey(pubkey, TWCoinTypeCosmos, hrp_utf8.get());
            auto twData = TWAnyAddressData(twAddress);
            auto hexData = hex(*reinterpret_cast<const Data*>(twData));
            ASSERT_EQ(hex(Bech32Address(addressParameters.hrp, TW::Hash::HasherSha256ripemd, pubkey->impl).getKeyHash()), hexData);
            auto address = TWAnyAddressDescription(twAddress);
            EXPECT_EQ(addressParameters.address, *reinterpret_cast<const std::string*>(address));
            TWStringDelete(address);
            TWAnyAddressDelete(twAddress);
            TWDataDelete(twData);
            TWPublicKeyDelete(pubkey);
        }

        // With coin type
        {
            auto publicKey = PublicKey(parse_hex(addressParameters.publicKey), addressParameters.publicKeyType);
            auto address = Address(addressParameters.coinType, publicKey);
        }
    }

    static inline void testCreateFromPrivKey(const CosmosAddressParameters& addressParameters) {
        auto privateKey = PrivateKey(parse_hex(addressParameters.privKey));
        auto address = Address(addressParameters.coinType, privateKey.getPublicKey(addressParameters.publicKeyType));
        ASSERT_EQ(address.string(), addressParameters.address);
    }

    static inline void testCreateFromString(const CosmosAddressParameters& addressParameters) {
        // BECH32
        if (addressParameters.standaloneChain) {
            const auto address = STRING(addressParameters.address.c_str());
            const auto hrp = STRING(addressParameters.hrp.c_str());
            const auto anyAddr = TWAnyAddressCreateBech32(address.get(), TWCoinTypeCosmos, hrp.get());
            const auto addrDescription = TWAnyAddressDescription(anyAddr);
            ASSERT_TRUE(TWAnyAddressIsValidBech32(addrDescription, TWCoinTypeCosmos, hrp.get()));
            TWStringDelete(addrDescription);
            TWAnyAddressDelete(anyAddr);
        }

        // With Coin Type
        {
            const auto address = STRING(addressParameters.address.c_str());
            const auto anyAddr = TWAnyAddressCreateWithString(address.get(), addressParameters.coinType);
            const auto addrDescription = TWAnyAddressDescription(anyAddr);
            ASSERT_TRUE(TWAnyAddressIsValid(addrDescription, addressParameters.coinType));
            TWStringDelete(addrDescription);
            TWAnyAddressDelete(anyAddr);
        }
    }
}

namespace TW::Cosmos::tests {
    static inline void TestCosmosAddressParameters(const CosmosAddressParameters& addressParameters) {
        internal::isValidAddress(addressParameters);
        internal::testCreateFromPubKeyWrapper(addressParameters);
        internal::testCreateFromPrivKey(addressParameters);
        internal::testCreateFromString(addressParameters);
    }
}
