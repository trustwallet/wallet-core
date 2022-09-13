// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "Data.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWKusamaAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

namespace TW::Kusama::tests {

const char* validAddrStr1 = "CeVXtoU4py9e7F6upfM2ZarVave299TjcdaTSxhDDZrYgnM";
const char* publicKeyDataStr1 = "032eb287017c5cde2940b5dd062d413f9d09f8aa44723fc80bf46b96c81ac23d";
const char* invalidAddrStr1 = "12345678";

TEST(TWKusamaAddress, CreateAndDelete) {
    {
        TWKusamaAddress* addr = TWKusamaAddressCreateWithString(STRING(validAddrStr1).get());
        EXPECT_TRUE(addr != nullptr);
        TWKusamaAddressDelete(addr);
    }
    {
        auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeED25519));
        TWKusamaAddress* addr = TWKusamaAddressCreateWithPublicKey(publicKey.get());
        EXPECT_TRUE(addr != nullptr);
        TWKusamaAddressDelete(addr);
    }
}

TEST(TWKusamaAddress, AddressEqual) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeED25519));
    auto addr1 = WRAP(TWKusamaAddress, TWKusamaAddressCreateWithPublicKey(publicKey.get()));
    EXPECT_TRUE(addr1.get() != nullptr);

    auto addr2 = WRAP(TWKusamaAddress, TWKusamaAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr2.get() != nullptr);
    ASSERT_TRUE(TWKusamaAddressEqual(addr1.get(), addr2.get()));
}

TEST(TWKusamaAddress, IsValidString) {
    ASSERT_TRUE(TWKusamaAddressIsValidString(STRING(validAddrStr1).get()));
    ASSERT_FALSE(TWKusamaAddressIsValidString(STRING(invalidAddrStr1).get()));
}

TEST(TWKusamaAddress, AddressDescription) {

    auto addr1 = WRAP(TWKusamaAddress, TWKusamaAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr1.get() != nullptr);
    auto addrStr1 = std::string(TWStringUTF8Bytes(WRAPS(TWKusamaAddressDescription(addr1.get())).get()));
    EXPECT_TRUE(addrStr1 == validAddrStr1);
}

} // namespace TW::Kusama::tests