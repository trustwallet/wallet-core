// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>

#include <gtest/gtest.h>


TEST(Nebulas, Address) {
    auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(DATA("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b").get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), false));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeNebulas));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    assertStringsEqual(addressString, "n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
}

TEST(Nebulas, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNebulas, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNebulas, TWHDVersionXPRV));

    // generate by https://iancoleman.io/bip39/#english
    assertStringsEqual(xpub, "xpub6BfZdeDKShWRToXqkSHsN3DN39LG59WgR4SWQoJRGLejQbfjGpsqNLgTNKkda47ykF3kpZ7ktEKPJfBR65QqBzrRfdEKPbhxQ7kxXaFWARU");
    assertStringsEqual(xprv, "xprv9xgDE8gRcKx8FKTNeQkrzuGdV7Vmfgnq3qWucQtoi17kXoLajHZapYMyX5Gg1qQ3isvekWzUJ7oTaD6U9hCYjz7mVYpRohvsgmwH8SRV7t2");
}

TEST(Nebulas, AddressEqual) {
    auto address1 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").get(), TWCoinTypeNebulas));
    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").get(), TWCoinTypeNebulas));
    auto address3 = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING("n1zUNqeBPvsyrw5zxp9mKcDdLTjuaEL7s39").get(), TWCoinTypeNebulas));
    EXPECT_TRUE(TWAnyAddressEqual(address1.get(),address2.get()));
    EXPECT_FALSE(TWAnyAddressEqual(address1.get(),address3.get()));
}

TEST(Nebulas, AddressIsValidString) {
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").get(), TWCoinTypeNebulas));
    EXPECT_TRUE(TWAnyAddressIsValid(STRING("n1zUNqeBPvsyrw5zxp9mKcDdLTjuaEL7s39").get(), TWCoinTypeNebulas));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("a1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").get(), TWCoinTypeNebulas));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("n2V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY").get(), TWCoinTypeNebulas));
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("n123").get(), TWCoinTypeNebulas));
}
