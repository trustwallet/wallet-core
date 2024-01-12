// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWRippleXAddress.h>

#include <gtest/gtest.h>

namespace TW::Ripple::tests {

const char* validAddrStr1 = "X76UnYEMbQfEs3mUqgtjp4zFy9exgThRj7XVZ6UxsdrBptF";
const char* publicKeyDataStr1 = "0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D";
const char* invalidAddrStr1 = "12345678";

TEST(TWRippleXAddress, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
                                       STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
                                       STRING("TREZOR").get()));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeXRP, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeXRP, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6D9oDY4gqFBtsFEonh5GTDiUm6nmij373YWzmYdshcnM4AFzdhUf55iZD33vNU2ZqfQJU5wiCJUgisMt2RHKDzhi1PbZfh5Y2NiiYJAQqUn");
    assertStringsEqual(xprv, "xprv9zASp2XnzsdbemALgfYG65mkD4xHKGKFgKbPyAEG9HFNBMvr6AAQXHQ5MmqM66EnbJfe9TvYMy1bucz7hSQjG43NVizRZwJJYfLmeKo4nVB");
}

TEST(TWRipple, XAddress) {
    const auto string = STRING("XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh");
    const auto xAddress = WRAP(TWRippleXAddress, TWRippleXAddressCreateWithString(string.get()));

    EXPECT_TRUE(TWRippleXAddressIsValidString(string.get()));
    EXPECT_EQ(TWRippleXAddressTag(xAddress.get()), 12345ul);
    assertStringsEqual(WRAPS(TWRippleXAddressDescription(xAddress.get())), "XVfvixWZQKkcenFRYApCjpTUyJ4BePTe3jJv7beatUZvQYh");
}

TEST(TWRippleXAddress, CreateAndDelete) {
    {
        TWRippleXAddress* addr = TWRippleXAddressCreateWithString(STRING(validAddrStr1).get());
        EXPECT_TRUE(addr != nullptr);
        TWRippleXAddressDelete(addr);
    }
    {
        auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeSECP256k1));
        TWRippleXAddress* addr = TWRippleXAddressCreateWithPublicKey(publicKey.get(), 12345);
        EXPECT_TRUE(addr != nullptr);
        TWRippleXAddressDelete(addr);
    }
}

TEST(TWRippleXAddress, AddressEqual) {
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(DATA(publicKeyDataStr1).get(), TWPublicKeyTypeSECP256k1));
    auto addr1 = WRAP(TWRippleXAddress, TWRippleXAddressCreateWithPublicKey(publicKey.get(), 12345));
    EXPECT_TRUE(addr1.get() != nullptr);

    auto addr2 = WRAP(TWRippleXAddress, TWRippleXAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr2.get() != nullptr);
    ASSERT_TRUE(TWRippleXAddressEqual(addr1.get(), addr2.get()));
}

TEST(TWRippleXAddress, IsValidString) {
    ASSERT_TRUE(TWRippleXAddressIsValidString(STRING(validAddrStr1).get()));
    ASSERT_FALSE(TWRippleXAddressIsValidString(STRING(invalidAddrStr1).get()));
}

TEST(TWRippleXAddress, AddressDescription) {

    auto addr1 = WRAP(TWRippleXAddress, TWRippleXAddressCreateWithString(STRING(validAddrStr1).get()));
    EXPECT_TRUE(addr1.get() != nullptr);
    auto addrStr1 = std::string(TWStringUTF8Bytes(WRAPS(TWRippleXAddressDescription(addr1.get())).get()));
    EXPECT_TRUE(addrStr1 == validAddrStr1);
}

} // namespace TW::Ripple::tests
