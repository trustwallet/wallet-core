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

    assertStringsEqual(addressStr, "EQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRR3n0");
}

} // namespace TW::TheOpenNetwork::tests
