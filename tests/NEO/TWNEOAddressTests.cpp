// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include "HexCoding.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

#include <iostream>


TEST(NEO, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("client recycle grass verb guitar battle abstract table they swamp accuse athlete recall ski light").get(),
        STRING("NEO").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNEO, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeNEO, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6CEwQUwgAnJmxNkb7CxuJGZN8FQNnqkKWeFjHqBEsD6PN267g3yNejdZyNEALzM7CxbQbtBzmndRjhvKyQDZoP8JrBLBQ8DJbhS1ge9Ln6F");
    assertStringsEqual(xprv, "xprv9yFazyQnLQkUjtg81BRtw8cdaDZtPP2U9RL8VSmdJsZQVDky8Wf86wK687witsCZhYZCaRALSbGRVbLBuzDzbp6dpJFqnjnvNbiNV4JgrNY");
}