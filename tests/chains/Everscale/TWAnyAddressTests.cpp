// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWHDWallet.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Everscale {

TEST(TWEverscale, HDWallet) {
    auto mnemonic =
        STRING("shoot island position soft burden budget tooth cruel issue economy destroy above");
    auto passphrase = STRING("");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), passphrase.get()));

    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKey(wallet.get(), TWCoinTypeEverscale, WRAPS(TWCoinTypeDerivationPath(TWCoinTypeEverscale)).get()));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKey.get()));
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeEverscale));
    auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressStr, "0:0c39661089f86ec5926ea7d4ee4223d634ba4ed6dcc2e80c7b6a8e6d59f79b04");
}

} // namespace TW::Everscale
