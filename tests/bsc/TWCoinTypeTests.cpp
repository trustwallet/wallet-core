// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

namespace TW::Binance::tests {

TEST(TWBinanceSmartChainLegacyCoinType, TWCoinType) {
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSmartChainLegacy));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSmartChainLegacy));

    ASSERT_EQ(10000714, TWCoinTypeSmartChainLegacy);
    assertStringsEqual(id, "bsc");
    assertStringsEqual(name, "Smart Chain Legacy");
}

} // namespace TW::Binance::tests
