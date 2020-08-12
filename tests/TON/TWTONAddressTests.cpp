// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWHDWallet.h>

#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

TEST(TWTONAddress, CreateWithString) {
    const char *expect = "EQCBVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODdZT";
    auto addrStr = STRING(expect);

    // first call isValid
    bool isValid = TWAnyAddressIsValid(addrStr.get(), TWCoinTypeTON);
    ASSERT_TRUE(isValid);

    // create address
    auto address = TWAnyAddressCreateWithString(addrStr.get(), TWCoinTypeTON);
    // convert back to string
    auto str2 = WRAPS(TWAnyAddressDescription(address));
    EXPECT_EQ(std::string(TWStringUTF8Bytes(addrStr.get())), std::string(TWStringUTF8Bytes(str2.get())));

    {
        // create a second one, also invoke compare
        auto address2 = TWAnyAddressCreateWithString(TWStringCreateWithUTF8Bytes(expect), TWCoinTypeTON);
        ASSERT_TRUE(TWAnyAddressEqual(address, address2));

        TWAnyAddressDelete(address2);
    }

    TWAnyAddressDelete(address);
}

TEST(TWTONAddress, CreateWithPublicKey) {
    auto pkData = DATA("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pkData.get(), TWPublicKeyTypeED25519));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeTON));
    auto addressStr = TWAnyAddressDescription(address.get());

    EXPECT_EQ(std::string("EQAkAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHUx3Y"), TWStringUTF8Bytes(addressStr));
}

TEST(TWTONAddress, HDWallet) {
    auto mnemonic = "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto privateKey = TWHDWalletGetKey(wallet.get(), TWCoinTypeTON, TWCoinTypeDerivationPath(TWCoinTypeTON));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey);
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey, TWCoinTypeTON));
    auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));

    ASSERT_EQ(std::string("EQAmXWk7P7avw96EViZULpA85Lz6Si3MeWG-vFXmbEjpL-fo"), TWStringUTF8Bytes(addressStr.get()));
}

TEST(TWTON, SigningNotImplemented) {
    // not implemented, returns empty data
    auto result = WRAPD(TWAnySignerSign(WRAPD(TWDataCreateWithSize(0)).get(), TWCoinType::TWCoinTypeTON));
    EXPECT_EQ(TWDataSize(result.get()), 0);
}
