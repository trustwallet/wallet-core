// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWAnyAddress.h>

#include <gtest/gtest.h>

TEST(Stellar, DeriveAddress) {
    auto words = STRING("indicate rival expand cave giant same grocery burden ugly rose tuna blood");
    auto passphrase = STRING("");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeStellar));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(key.get()));

    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(publicKey.get(), TWCoinTypeStellar));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));

    assertStringsEqual(addressString, "GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
}
