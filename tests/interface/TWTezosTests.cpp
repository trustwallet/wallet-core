// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWTezosAddress.h>

#include <gtest/gtest.h>

TEST(Tezos, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeTezos, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeTezos, TWHDVersionXPRV));

    assertStringsEqual(xpub, "tecKXJyGRqJBWjz1VSKzDzfhbqWwGkiaR95asC4AUn6sND2h5q");
    assertStringsEqual(xprv, "Rt6N8LzcnTDo13RPddhqew8DJmUWkjzzCeifDsGBzRckFFPkMFi4jPoYqn1sx4qq6BpgabD1dCMUCyrfrcKXjV5r4omeeY");
}

TEST(Tezos, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6D9oDY4gqFBtsFEonh5GTDiUm6nmij373YWzmYdshcnM4AFzdhUf55iZD33vNU2ZqfQJU5wiCJUgisMt2RHKDzhi1PbZfh5Y2NiiYJAQqUn");
    auto pubKey2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCurveEd25519, TWHDVersionXPUB, TWHDVersionXPRV, 0, 2);
    auto pubKey6 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCurveEd25519, TWHDVersionXPUB, TWHDVersionXPRV, 0, 6);

    auto address2 = TWTezosAddressCreateWithPublicKey(pubKey2);
    auto address2String = WRAPS(TWTezosAddressDescription(address2));

    auto address6 = TWTezosAddressCreateWithPublicKey(pubKey6);
    auto address6String = WRAPS(TWTezosAddressDescription(address6));

    assertStringsEqual(address2String, "rJD8Bobd1JDJW34Y2nyqxofvfe672AGw7e");
    assertStringsEqual(address6String, "rLimL8BjBDb1UB8icRtTnJTt5m6DUeWXbC");
}

TEST(Tezos, DeriveAddressFromXpub) {
    auto xpub = STRING("xpub6D9oDY4gqFBtsFEonh5GTDiUm6nmij373YWzmYdshcnM4AFzdhUf55iZD33vNU2ZqfQJU5wiCJUgisMt2RHKDzhi1PbZfh5Y2NiiYJAQqUn");
    auto address = WRAPS(TWHDWalletGetAddressFromExtended(xpub.get(), TWCurveEd25519, TWCoinTypeTezos, 0, 0));

    assertStringsEqual(address, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav");
}
