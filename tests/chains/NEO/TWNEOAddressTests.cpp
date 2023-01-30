// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWNEOAddress.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::NEO::tests {

const char* validAddrStr1 = "AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5";
const char* publicKeyDataStr1 = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
const char* invalidAddrStr1 = "12345678";

TEST(TWNEOAddress, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
                                       STRING("client recycle grass verb guitar battle abstract table they swamp accuse athlete recall ski light").get(),
                                       STRING("NEO").get()));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNEO, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNEO, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6CEwQUwgAnJmxNkb7CxuJGZN8FQNnqkKWeFjHqBEsD6PN267g3yNejdZyNEALzM7CxbQbtBzmndRjhvKyQDZoP8JrBLBQ8DJbhS1ge9Ln6F");
    assertStringsEqual(xprv, "xprv9yFazyQnLQkUjtg81BRtw8cdaDZtPP2U9RL8VSmdJsZQVDky8Wf86wK687witsCZhYZCaRALSbGRVbLBuzDzbp6dpJFqnjnvNbiNV4JgrNY");
}

TEST(TWNEOAddress, CreateAndDelete) {
    {
        TWNEOAddress* addr = TWNEOAddressCreateWithString(STRING(validAddrStr1).get());
        EXPECT_TRUE(addr != nullptr);
        TWNEOAddressDelete(addr);
    }
    {
        auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeNIST256p1));
        TWNEOAddress* addr = TWNEOAddressCreateWithPublicKey(publicKey.get());
        EXPECT_TRUE(addr != nullptr);
        TWNEOAddressDelete(addr);
    }
}

TEST(TWNEOAddress, AddressEqual) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeNIST256p1));
    auto addr1 = WRAP(TWNEOAddress, TWNEOAddressCreateWithPublicKey(publicKey.get()));
    EXPECT_TRUE(addr1.get() != nullptr);

    auto addr2 = WRAP(TWNEOAddress, TWNEOAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr2.get() != nullptr);
    ASSERT_TRUE(TWNEOAddressEqual(addr1.get(), addr2.get()));
}

TEST(TWNEOAddress, IsValidString) {
    ASSERT_TRUE(TWNEOAddressIsValidString(STRING(validAddrStr1).get()));
    ASSERT_FALSE(TWNEOAddressIsValidString(STRING(invalidAddrStr1).get()));
}

TEST(TWNEOAddress, AddressDescription) {

    auto addr1 = WRAP(TWNEOAddress, TWNEOAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr1.get() != nullptr);
    auto addrStr1 = std::string(TWStringUTF8Bytes(WRAPS(TWNEOAddressDescription(addr1.get())).get()));
    EXPECT_TRUE(addrStr1 == validAddrStr1);
}

} // namespace TW::NEO::tests