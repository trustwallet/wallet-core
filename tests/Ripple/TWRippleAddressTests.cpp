// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

TEST(TWRipple, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeXRP, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeXRP, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6D9oDY4gqFBtsFEonh5GTDiUm6nmij373YWzmYdshcnM4AFzdhUf55iZD33vNU2ZqfQJU5wiCJUgisMt2RHKDzhi1PbZfh5Y2NiiYJAQqUn");
    assertStringsEqual(xprv, "xprv9zASp2XnzsdbemALgfYG65mkD4xHKGKFgKbPyAEG9HFNBMvr6AAQXHQ5MmqM66EnbJfe9TvYMy1bucz7hSQjG43NVizRZwJJYfLmeKo4nVB");
}
