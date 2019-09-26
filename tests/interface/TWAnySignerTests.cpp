// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "TWTestUtilities.h"

#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWAnySigner, IsEnabled) {
    ASSERT_TRUE(TWAnySignerIsSignEnabled(TWCoinTypeEthereum));
    ASSERT_TRUE(TWAnySignerIsSignEnabled(TWCoinTypeBinance));

    ASSERT_FALSE(TWAnySignerIsSignEnabled(TWCoinTypeBitcoinCash));
    ASSERT_FALSE(TWAnySignerIsSignEnabled(TWCoinTypeBitcoin));
}
