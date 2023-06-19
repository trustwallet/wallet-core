// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include "Data.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSubstrateAddress.h>

#include <gtest/gtest.h>

namespace TW::Substrate::tests {

const char* validAddrStr1 = "14KjL5vGAYJCbKgZJmFKDSjewtBpvaxx9YvRZvi7qmb5s8CC";
const char* publicKeyDataStr1 = "92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3";
const char* invalidAddrStr1 = "12345678";

TEST(TWSubstrateAddress, CreateAndDelete) {
    {
        TWSubstrateAddress* addr = TWSubstrateAddressCreateWithString(STRING(validAddrStr1).get(), 0);
        EXPECT_TRUE(addr != nullptr);
        TWSubstrateAddressDelete(addr);
    }
    {
        auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeED25519));
        TWSubstrateAddress* addr = TWSubstrateAddressCreateWithPublicKey(publicKey.get(), 0);
        EXPECT_TRUE(addr != nullptr);
        TWSubstrateAddressDelete(addr);
    }
}

TEST(TWSubstrateAddress, AddressEqual) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeED25519));
    auto addr1 = WRAP(TWSubstrateAddress, TWSubstrateAddressCreateWithPublicKey(publicKey.get(), 0));
    EXPECT_TRUE(addr1.get() != nullptr);

    auto addr2 = WRAP(TWSubstrateAddress, TWSubstrateAddressCreateWithString(STRING(validAddrStr1).get(), 0));
    EXPECT_TRUE(addr2.get() != nullptr);
    ASSERT_TRUE(TWSubstrateAddressEqual(addr1.get(), addr2.get()));
}

TEST(TWSubstrateAddress, IsValidString) {
    ASSERT_TRUE(TWSubstrateAddressIsValidString(STRING(validAddrStr1).get(), 0));
    ASSERT_FALSE(TWSubstrateAddressIsValidString(STRING(invalidAddrStr1).get(), 0));
}

TEST(TWSubstrateAddress, AddressDescription) {

    auto addr1 = WRAP(TWSubstrateAddress, TWSubstrateAddressCreateWithString(STRING(validAddrStr1).get(), 0));
    EXPECT_TRUE(addr1.get() != nullptr);
    auto addrStr1 = std::string(TWStringUTF8Bytes(WRAPS(TWSubstrateAddressDescription(addr1.get())).get()));
    EXPECT_TRUE(addrStr1 == validAddrStr1);
}

} // namespace TW::Substrate::tests