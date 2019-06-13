// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TWTestUtilities.h"
#include "proto/Aeternity.pb.h"
#include "HDWallet.h"

#include <TrustWalletCore/TWAeternityAddress.h>
#include <TrustWalletCore/TWAeternitySigner.h>
#include <TrustWalletCore/TWAeternityProto.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWAeternityAddress, HDWallet) {
    auto mnemonic = "shoot island position soft burden budget tooth cruel issue economy destroy above";
    auto passphrase = "";

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
            STRING(mnemonic).get(),
            STRING(passphrase).get()
    ));

    auto privateKey = TWHDWalletGetKey(wallet.get(), TWCoinTypeDerivationPath(TWCoinTypeAeternity));
    auto publicKey = TWPrivateKeyGetPublicKeyEd25519(privateKey);
    auto address = TWAeternityAddressCreateWithPublicKey(publicKey);
    auto addressStr = WRAPS(TWAeternityAddressDescription(address));

    assertStringsEqual(addressStr, "ak_QDHJSfvHG9sDHBobaWt2TAGhuhipYjEqZEH34bWugpJfJc3GN");
}