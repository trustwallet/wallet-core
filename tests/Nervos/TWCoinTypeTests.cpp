// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWNervosCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNervos));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNervos));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNervos));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNervos), 8);
    ASSERT_EQ(TWBlockchainNervos, TWCoinTypeBlockchain(TWCoinTypeNervos));
    assertStringsEqual(symbol, "CKB");
    assertStringsEqual(id, "nervos");
    assertStringsEqual(name, "Nervos");
}
