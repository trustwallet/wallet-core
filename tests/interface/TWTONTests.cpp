// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTONAddress.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWHDWallet.h>

#include "TWTestUtilities.h"

#include <gtest/gtest.h>

TEST(TWTONAddress, CreateWithString) {
    auto addrStr = TWStringCreateWithUTF8Bytes("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb");

    // first call isValid
    bool isValid = TWTONAddressIsValidString(addrStr);
    ASSERT_TRUE(isValid);

    // create address
    auto address = TWTONAddressCreateWithString(addrStr);
    // convert back to string
    auto str2 = TWTONAddressDescription(address);
    ASSERT_EQ(std::string(TWStringUTF8Bytes(addrStr)), std::string(TWStringUTF8Bytes(str2)));

    {
        // create a second one, also invoke compare
        auto address2 = TWTONAddressCreateWithString(TWStringCreateWithUTF8Bytes("Ef-BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb"));
        ASSERT_TRUE(TWTONAddressEqual(address, address2));

        TWTONAddressDelete(address2);
    }

    TWTONAddressDelete(address);
}

TEST(TWTONAddress, CreateWithPublicKey) {
    auto pkData = DATA("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pkData.get(), TWPublicKeyTypeED25519));
    auto address = WRAP(TWTONAddress, TWTONAddressCreateWithPublicKey(publicKey.get()));
    auto addressStr = TWTONAddressDescription(address.get());

    ASSERT_EQ(std::string("Ef9gwEFBxqe5bWhhXnqR0mWtDzqaki6a6ckB1PqD9dPA0KTM"), TWStringUTF8Bytes(addressStr));
}

TEST(TWTONAddress, HDWallet) {
    auto mnemonic = "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING(mnemonic).get(), STRING(passphrase).get()));

    auto privateKey = TWHDWalletGetKey(wallet.get(), TWCoinTypeDerivationPath(TWCoinTypeTON));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey);
    auto address = TWTONAddressCreateWithPublicKey(publicKey);
    auto addressStr = WRAPS(TWTONAddressDescription(address));

    assertStringsEqual(addressStr, "Ef9D0h+JxF6dTWY1tgwduk+ZZcCr0AVu/2t01g5IkKEsiYQJ");
}
