// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Data.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPolkadotAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

namespace TW::Polkadolt::tests {

const char* validAddrStr1 = "15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu";
const char* publicKeyDataStr1 = "beff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908";
const char* invalidAddrStr1 = "12345678";

TEST(TWPolkadotAddress, CreateAndDelete) {
    {
        TWPolkadotAddress* addr = TWPolkadotAddressCreateWithString(STRING(validAddrStr1).get());
        EXPECT_TRUE(addr != nullptr);
        TWPolkadotAddressDelete(addr);
    }
    {
        auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeED25519));
        TWPolkadotAddress* addr = TWPolkadotAddressCreateWithPublicKey(publicKey.get());
        EXPECT_TRUE(addr != nullptr);
        TWPolkadotAddressDelete(addr);
    }
}

TEST(TWPolkadotAddress, AddressEqual) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeED25519));
    auto addr1 = WRAP(TWPolkadotAddress, TWPolkadotAddressCreateWithPublicKey(publicKey.get()));
    EXPECT_TRUE(addr1.get() != nullptr);

    auto addr2 = WRAP(TWPolkadotAddress, TWPolkadotAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr2.get() != nullptr);
    ASSERT_TRUE(TWPolkadotAddressEqual(addr1.get(), addr2.get()));
}

TEST(TWPolkadotAddress, IsValidString) {
    ASSERT_TRUE(TWPolkadotAddressIsValidString(STRING(validAddrStr1).get()));
    ASSERT_FALSE(TWPolkadotAddressIsValidString(STRING(invalidAddrStr1).get()));
}

TEST(TWPolkadotAddress, AddressDescription) {

    auto addr1 = WRAP(TWPolkadotAddress, TWPolkadotAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr1.get() != nullptr);
    auto addrStr1 = std::string(TWStringUTF8Bytes(WRAPS(TWPolkadotAddressDescription(addr1.get())).get()));
    EXPECT_TRUE(addrStr1 == validAddrStr1);
}

} // namespace TW::Polkadolt::tests
