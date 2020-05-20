// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TxComparisonHelper.h"
#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/FeeCalculator.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(TransactionPlan, NonMaxAmount) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 50000, 120000});
    auto sigingInput = buildSigningInput(10000, 1, utxos, false, TWCoinTypeBitcoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_EQ(verifyPlan(txPlan, {50000}, 10000, 226), "");
}

TEST(TransactionPlan, MaxAmount) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200);
    auto byteFee = 32;
    auto sigingInput = buildSigningInput(39200, byteFee, utxos, true, TWCoinTypeBitcoin);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4736);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_EQ(verifyPlan(txPlan, {15000, 15000}, 19120, 10880), "");
}

TEST(TransactionPlan, MaxAmountLowerRequested) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200);
    auto byteFee = 32;
    auto sigingInput = buildSigningInput(10, byteFee, utxos, true, TWCoinTypeBitcoin);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4736);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_EQ(verifyPlan(txPlan, {15000, 15000}, 19120, 10880), "");
}

TEST(TransactionPlan, MaxAmountDoge) {
    auto utxos = buildTestUTXOs({Amount(100000000), Amount(2000000000), Amount(200000000)});
    ASSERT_EQ(sumUTXOs(utxos), Amount(2300000000));
    auto sigingInput = buildSigningInput(Amount(2300000000), 100, utxos, true, TWCoinTypeDogecoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_EQ(verifyPlan(txPlan, {100000000, 200000000, 2000000000}, 2299951200, 48800), "");
}

TEST(TransactionPlan, AmountDecred) {
    auto utxos = buildTestUTXOs({Amount(39900000)});
    auto sigingInput = buildSigningInput(Amount(10000000), 10, utxos, false, TWCoinTypeDecred);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_EQ(verifyPlan(txPlan, {39900000}, 10000000, 2540), "");
}
