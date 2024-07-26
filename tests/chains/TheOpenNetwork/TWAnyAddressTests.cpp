// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::TheOpenNetwork::tests {

TEST(TWTheOpenNetwork, Address) {
    const auto mnemonic = STRING("stuff diamond cycle federal scan spread pigeon people engage teach snack grain");
    const auto passphrase = STRING("");

    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), passphrase.get()));

    const auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeTON, WRAPS(TWCoinTypeDerivationPath(TWCoinTypeTON)).get()));
    const auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey.get()));
    const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeTON));
    const auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressStr, "UQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRRyQx");
}

TEST(TWTheOpenNetwork, AddressValidate) {
    auto string = STRING("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl");

    ASSERT_TRUE(TWAnyAddressIsValid(string.get(), TWCoinTypeTON));
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeTON));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");

    auto normalized = WRAPS(TWAnyAddressDescription(addr.get()));
    assertStringsEqual(normalized, "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg");
}

} // namespace TW::TheOpenNetwork::tests
