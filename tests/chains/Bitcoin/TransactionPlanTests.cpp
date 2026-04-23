// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TxComparisonHelper.h"
#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/FeeCalculator.h"
#include "HexCoding.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(TransactionPlan, OneTypical) {
    auto utxos = buildTestUTXOs({100'000});
    const Amount byteFee = 1ull;
    auto sigingInput = buildSigningInput(50'000ull, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 50'000ull, 147ull));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 174ull);
}

TEST(TransactionPlan, OneInsufficient) {
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(200'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // Max is returned
    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 99'887ull, 113ull));
}

TEST(TransactionPlan, OneInsufficientEqual) {
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // Max is returned
    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 99'887ull, 113ull));
}

TEST(TransactionPlan, OneInsufficientLower100) {
    // requested is only slightly lower than avail, not enough for fee, cannot be satisfied
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000ull - 100ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0ull, 0ull, Common::Proto::Error_not_enough_utxos));
}

TEST(TransactionPlan, OneInsufficient146) {
    // requested is only slightly lower than avail, not enough for fee, cannot be satisfied
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000ull - 146ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0ull, 0ull, Common::Proto::Error_not_enough_utxos));
}

TEST(TransactionPlan, OneSufficientLower170) {
    // requested is only slightly lower than avail, not enough for fee, cannot be satisfied
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000ull - 170ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount dustChange = 23ull;
    const Amount actualFee = 147ull + dustChange;
    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000ull - 170ull, actualFee));
}

TEST(TransactionPlan, OneSufficientLower300) {
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000ull - 300ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000ull - 300ull, 147ull));
}

TEST(TransactionPlan, OneMoreRequested) {
    auto utxos = buildTestUTXOs({100'000});
    const Amount byteFee = 1ull;
    auto sigingInput = buildSigningInput(150'000ull, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // Max is returned
    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 99'887ull, 113ull));
}

TEST(TransactionPlan, OneFitsExactly) {
    auto utxos = buildTestUTXOs({100'000});
    const Amount byteFee = 1ull;
    const Amount dustChange = 27ull;
    // Change amount is too low (less than dust), so we just waste it as the transaction fee.
    const Amount expectedFee = 147ull + dustChange;
    auto sigingInput = buildSigningInput(100'000ull - 174ull, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000ull - 174ull, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 174ull);
}

TEST(TransactionPlan, OneFitsExactlyHighFee) {
    auto utxos = buildTestUTXOs({100'000});
    const Amount byteFee = 10ull;
    const Amount dustChange = 270ull;
    // Change amount is too low (less than dust), so we just waste it as the transaction fee.
    const Amount expectedFee = 1470ull + dustChange;
    auto sigingInput = buildSigningInput(100'000ull - 1740ull, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000ull - 1740ull, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 1740ull);
}

TEST(TransactionPlan, OneMissingPrivateKey) {
    auto utxos = buildTestUTXOs({100'000});
    const Amount byteFee = 1ull;
    auto sigingInput = buildSigningInput(50'000ull, byteFee, utxos, false, TWCoinTypeBitcoin, true);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 50'000ull, 147ull));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 174ull);
}

TEST(TransactionPlan, TwoFirstEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});
    auto sigingInput = buildSigningInput(15'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {20'000}, 15'000ull, 147ull));
}

TEST(TransactionPlan, TwoSecondEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});
    auto sigingInput = buildSigningInput(70'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {80'000}, 70'000ull, 147ull));
}

TEST(TransactionPlan, TwoBoth) {
    auto utxos = buildTestUTXOs({20'000, 80'000});
    auto sigingInput = buildSigningInput(90'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {20'000, 80'000}, 90'000ull, 215ull));
}

TEST(TransactionPlan, TwoFirstEnoughButSecond) {
    auto utxos = buildTestUTXOs({20'000, 22'000});
    auto sigingInput = buildSigningInput(18'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {22'000}, 18'000ull, 147ull));
}

TEST(TransactionPlan, ThreeNoDust) {
    auto utxos = buildTestUTXOs({100'000, 70'000, 75'000});
    auto sigingInput = buildSigningInput(100'000ull - 174ull - 10ull, 1ull, utxos);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {75'000, 100'000}, 100'000ull - 174ull - 10ull, 215ull));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1ull), 174ull);
    EXPECT_EQ(feeCalculator.calculate(2, 2, 1ull), 275ull);

    const Amount dustLimit = 102ull;
    // Now 100'000 fits with no dust
    sigingInput = buildSigningInput(100'000ull - 174ull - dustLimit, 1ull, utxos);
    txPlan = TransactionBuilder::plan(sigingInput);
    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000ull - 174ull - dustLimit, 147ull));

    // One more and we are over dust limit
    sigingInput = buildSigningInput(100'000ull - 174ull - dustLimit + 1ull, 1ull, utxos);
    txPlan = TransactionBuilder::plan(sigingInput);
    EXPECT_TRUE(verifyPlan(txPlan, {75'000, 100'000}, 100'000ull - 174ull - dustLimit + 1ull, 215ull));
}

TEST(TransactionPlan, TenThree) {
    // Note: was `5, 000` (typo) — fixed to `5'000`
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5'000, 125'000, 6'000, 150'000, 7'000});
    auto sigingInput = buildSigningInput(300'000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000, 125'000, 150'000}, 300'000ull, 283ull));
}

TEST(TransactionPlan, NonMaxAmount) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 50000, 120000});
    auto sigingInput = buildSigningInput(10000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {50000}, 10000ull, 147ull));
}

TEST(TransactionPlan, UnspentsInsufficient) {
    auto utxos = buildTestUTXOs({4000, 4000, 4000});
    auto sigingInput = buildSigningInput(15000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // Max is returned
    EXPECT_TRUE(verifyPlan(txPlan, {4000, 4000, 4000}, 11751ull, 249ull));
}

TEST(TransactionPlan, SelectionSuboptimal_ExtraSmallUtxo) {
    // Solution found 4-in-2-out {500, 600, 800, 1000} avail 2900 txamount 1570 fee 702 change 628
    // Better solution: 3-in-2-out {600, 800, 1000} avail 2400 txamount 1570 fee 566 change 264
    // Previously, with with higher fee estimation used in UTXO selection, solution found was 5-in-2-out {400, 500, 600, 800, 1000} avail 3300 txamount 1570 fee 838 change 892
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1'000});
    const Amount byteFee = 2ull;
    auto sigingInput = buildSigningInput(1'570ull, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 702ull;
    EXPECT_TRUE(verifyPlan(txPlan, {500, 600, 800, 1'000}, 1'570ull, expectedFee));
    const Amount change = 2'900ull - 1'570ull - expectedFee;
    const Amount firstUtxo = txPlan.utxos[0].amount;
    EXPECT_TRUE(change - 204ull < txPlan.utxos[0].amount);
    EXPECT_EQ(change, 628ull);
    EXPECT_EQ(firstUtxo, 500ull);
}

TEST(TransactionPlan, SelectionSuboptimal_ExtraSmallUtxoFixedDust) {
    // Solution found 4-in-2-out {500, 600, 800, 1000} avail 2900 txamount 1390 fee 702 change 628
    // Better solution: 3-in-2-out {600, 800, 1000} avail 2400 txamount 1390 fee 566 change 444
    // Previously, with with higher fee estimation used in UTXO selection, solution found was 5-in-2-out {400, 500, 600, 800, 1000} avail 3300 txamount 1390 fee 838 change 1072
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1'000});
    const Amount byteFee = 2ull;
    auto signingInput = buildSigningInput(1'390ull, byteFee, utxos);
    signingInput.dustCalculator = std::make_shared<FixedDustCalculator>(450ull);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(signingInput);

    const Amount expectedFee = 702ull;
    EXPECT_TRUE(verifyPlan(txPlan, {500, 600, 800, 1'000}, 1'390ull, expectedFee));
    const Amount change = 2'900ull - 1'390ull - expectedFee;
    const Amount firstUtxo = txPlan.utxos[0].amount;
    EXPECT_EQ(change, 808ull);
    EXPECT_EQ(firstUtxo, 500ull);
}

TEST(TransactionPlan, Selection_Satisfied5) {
    // 5-input case, with a 5-input solution.
    // Previously, with with higher fee estimation used in UTXO selection, no solution would be found.
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1'000});
    const Amount byteFee = 2ull;
    auto sigingInput = buildSigningInput(1'775ull, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {400, 500, 600, 800, 1000}, 1775ull, 838ull));
}

TEST(TransactionPlan, Inputs5_33Req19NoDustFee2) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    const Amount byteFee = 2ull;
    auto sigingInput = buildSigningInput(19'000ull, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 283ull * byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 19'000ull, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 204ull);
}

TEST(TransactionPlan, Inputs5_33Req19Dust1Fee5) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    const Amount byteFee = 5ull;
    auto sigingInput = buildSigningInput(19'000ull, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 283ull * byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 19'000ull, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 510ull);
}

TEST(TransactionPlan, Inputs5_33Req19Dust1Fee9) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    const Amount byteFee = 9ull;
    auto sigingInput = buildSigningInput(19'000ull, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 283ull * byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 19'000ull, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 918ull);
}

TEST(TransactionPlan, Inputs5_33Req19Fee20) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    const Amount byteFee = 20ull;
    auto sigingInput = buildSigningInput(19'000ull, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0ull, 0ull, Common::Proto::Error_not_enough_utxos));
}

TEST(TransactionPlan, Inputs5_33Req13Fee20) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    const Amount byteFee = 20ull;
    auto sigingInput = buildSigningInput(13'000ull, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 283ull * byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 13'000ull, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 2040ull);
}

TEST(TransactionPlan, NoUTXOs) {
    auto utxos = buildTestUTXOs({});
    auto sigingInput = buildSigningInput(15000ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0ull, 0ull, Common::Proto::Error_missing_input_utxos));
}

TEST(TransactionPlan, CustomCase) {
    auto utxos = buildTestUTXOs({794121, 2289357});
    const Amount byteFee = 61ull;
    auto sigingInput = buildSigningInput(2287189ull, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {794121, 2289357}, 2287189ull, 13115ull));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(2, 2, byteFee), 16775ull);
}

TEST(TransactionPlan, Target0) {
    auto utxos = buildTestUTXOs({2000, 3000});
    auto sigingInput = buildSigningInput(0ull, 1ull, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0ull, 0ull, Common::Proto::Error_zero_amount_requested));
}

TEST(TransactionPlan, MaxAmount) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200ull);
    const Amount byteFee = 40ull;
    auto sigingInput = buildSigningInput(39200ull, byteFee, utxos, true);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4080ull);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 7240ull;
    EXPECT_TRUE(verifyPlan(txPlan, {15000, 15000}, 30000ull - expectedFee, expectedFee));
}

TEST(TransactionPlan, MaxAmountOne) {
    auto utxos = buildTestUTXOs({10189534});
    auto sigingInput = buildSigningInput(100ull, 1ull, utxos, true);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 113ull;
    EXPECT_TRUE(verifyPlan(txPlan, {10189534}, 10189534ull - expectedFee, expectedFee));
}

TEST(TransactionPlan, AmountEqualsMaxButNotUseMax) {
    // amount is set to max, but UseMax is not set --> Max is returned
    auto utxos = buildTestUTXOs({10189534});
    auto sigingInput = buildSigningInput(10189534ull, 1ull, utxos, false);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {10189534}, 10189421ull, 113ull));
}

TEST(TransactionPlan, MaxAmountRequestedIsLower) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200ull);
    const Amount byteFee = 40ull;
    auto sigingInput = buildSigningInput(10ull, byteFee, utxos, true);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4080ull);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 7240ull;
    EXPECT_TRUE(verifyPlan(txPlan, {15000, 15000}, 30000ull - expectedFee, expectedFee));
}

TEST(TransactionPlan, MaxAmountRequestedZero) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200ull);
    const Amount byteFee = 40ull;
    auto sigingInput = buildSigningInput(0ull, byteFee, utxos, true);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4080ull);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 7240ull;
    EXPECT_TRUE(verifyPlan(txPlan, {15000, 15000}, 30000ull - expectedFee, expectedFee));
}

TEST(TransactionPlan, MaxAmountNoDustFee2) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    const Amount byteFee = 2ull;
    auto sigingInput = buildSigningInput(100ull, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 770ull;
    EXPECT_TRUE(verifyPlan(txPlan, {400, 500, 600, 800, 1000}, 3'300ull - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 204ull);
    EXPECT_EQ(feeCalculator.calculate(5, 1, byteFee), 1096ull);
}

TEST(TransactionPlan, MaxAmountDust1Fee4) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    const Amount byteFee = 4ull;
    auto sigingInput = buildSigningInput(100ull, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 1268ull;
    EXPECT_TRUE(verifyPlan(txPlan, {500, 600, 800, 1000}, 2'900ull - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 408ull);
    EXPECT_EQ(feeCalculator.calculate(4, 1, byteFee), 1784ull);
}

TEST(TransactionPlan, MaxAmountDust2Fee5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    const Amount byteFee = 5ull;
    auto sigingInput = buildSigningInput(100ull, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    const Amount expectedFee = 1245ull;
    EXPECT_TRUE(verifyPlan(txPlan, {600, 800, 1000}, 2'400ull - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 510ull);
    EXPECT_EQ(feeCalculator.calculate(3, 1, byteFee), 1725ull);
}

TEST(TransactionPlan, MaxAmountDustAllFee10) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    const Amount byteFee = 10ull;
    auto sigingInput = buildSigningInput(100ull, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0ull, 0ull, Common::Proto::Error_not_enough_utxos));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 1020ull);
}

TEST(TransactionPlan, One_MaxAmount_FeeMoreThanAvailable) {
    auto utxos = buildTestUTXOs({340});
    const Amount byteFee = 1ull;
    const Amount expectedFee = 113ull;
    auto sigingInput = buildSigningInput(340ull, byteFee, utxos, true);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // Fee is reduced to availableAmount
    EXPECT_TRUE(verifyPlan(txPlan, {340}, 340ull - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 1, byteFee), 143ull);
}

TEST(TransactionPlan, MaxAmountDoge) {
    auto utxos = buildTestUTXOs({100000000, 2000000000, 200000000});
    ASSERT_EQ(sumUTXOs(utxos), 2300000000ull);
    auto sigingInput = buildSigningInput(2300000000ull, 100ull, utxos, true, TWCoinTypeDogecoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100000000, 2000000000, 200000000}, 2299951200ull, 48800ull));
}

TEST(TransactionPlan, AmountDecred) {
    auto utxos = buildTestUTXOs({39900000});
    auto sigingInput = buildSigningInput(10000000ull, 10ull, utxos, false, TWCoinTypeDecred);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {39900000}, 10000000ull, 2540ull));
}

TEST(TransactionPlan, ManyUtxosNonmax_400) {
    const auto n = 400;
    const Amount byteFee = 10ull;
    std::vector<Amount> values;
    Amount valueSum = 0ull;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        values.push_back(val);
        valueSum += val;
    }
    const Amount requestedAmount = valueSum / 8ull;
    EXPECT_EQ(requestedAmount, 1'002'500ull);

    auto utxos = buildTestUTXOs(values);
    auto sigingInput = buildSigningInput(requestedAmount, byteFee, utxos, false, TWCoinTypeBitcoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // expected result: 27 utxos, with the largest amounts
    std::vector<Amount> subset;
    Amount subsetSum = 0ull;
    for (int i = n - 27; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 27ull);
    EXPECT_EQ(subsetSum, 1'044'900ull);
    EXPECT_TRUE(verifyPlan(txPlan, subset, requestedAmount, 19'150ull));
}

TEST(TransactionPlan, ManyUtxosNonmax_5000_simple) {
    const auto n = 5000;
    const Amount byteFee = 10ull;
    std::vector<Amount> values;
    Amount valueSum = 0ull;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        values.push_back(val);
        valueSum += val;
    }
    const Amount requestedAmount = valueSum / 20ull;
    EXPECT_EQ(requestedAmount, 62'512'500ull);

    // Use Ravencoin, because of faster non-segwit estimation, and one of the original issues was with this coin.
    auto utxos = buildTestUTXOs(values);
    auto sigingInput = buildSigningInput(requestedAmount, byteFee, utxos, false, TWCoinTypeRavencoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // expected result: 1220 utxos, with the smaller amounts (except the very small dust ones)
    std::vector<Amount> subset;
    Amount subsetSum = 0ull;
    for (int i = 14; i < 1220 + 14; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 1220ull);
    EXPECT_EQ(subsetSum, 76'189'000ull);
    EXPECT_TRUE(verifyPlan(txPlan, subset, requestedAmount, 1'806'380ull));
}

TEST(TransactionPlan, ManyUtxosMax_400) {
    const auto n = 400;
    const Amount byteFee = 10ull;
    std::vector<Amount> values;
    Amount valueSum = 0ull;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        values.push_back(val);
        valueSum += val;
    }

    // Use Ravencoin, because of faster non-segwit estimation, and one of the original issues was with this coin.
    auto utxos = buildTestUTXOs(values);
    auto sigingInput = buildSigningInput(valueSum, byteFee, utxos, true, TWCoinTypeRavencoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // all are selected, except a few smallest UTXOs are filtered out
    const Amount dustLimit = byteFee * 148ull;
    std::vector<Amount> filteredValues;
    Amount filteredValueSum = 0ull;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        if (val > dustLimit) {
            filteredValues.push_back(val);
            filteredValueSum += val;
        }
    }
    EXPECT_EQ(valueSum, 8'020'000ull);
    EXPECT_EQ(dustLimit, 1480ull);
    EXPECT_EQ(filteredValues.size(), 386ull);
    EXPECT_EQ(filteredValueSum, 8'009'500ull);
    EXPECT_TRUE(verifyPlan(txPlan, filteredValues, 7'437'780ull, 571'720ull));
}

TEST(TransactionPlan, ManyUtxosMax_5000_simple) {
    const auto n = 5000;
    const Amount byteFee = 10ull;
    std::vector<Amount> values;
    Amount valueSum = 0ull;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        values.push_back(val);
        valueSum += val;
    }

    // Use Ravencoin, because of faster non-segwit estimation, and one of the original issues was with this coin.
    auto utxos = buildTestUTXOs(values);
    auto sigingInput = buildSigningInput(valueSum, byteFee, utxos, true, TWCoinTypeRavencoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // only 3000 are selected, the first ones minus a few small dust ones
    const Amount dustLimit = byteFee * 150ull;
    std::vector<Amount> filteredValues;
    Amount filteredValueSum = 0ull;
    for (int i = 0; i < 3000 + 14; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        if (val >= dustLimit) {
            filteredValues.push_back(val);
            filteredValueSum += val;
        }
    }
    EXPECT_EQ(valueSum, 1'250'250'000ull);
    EXPECT_EQ(dustLimit, 1500ull);
    EXPECT_EQ(filteredValues.size(), 3000ull);
    EXPECT_EQ(filteredValueSum, 454'350'000ull);
    EXPECT_TRUE(verifyPlan(txPlan, filteredValues, 449'909'560ull, 4'440'440ull));
}

TEST(TransactionPlan, OpReturn) {
    auto ownAddress = "bc1q7s0a2l4aguksehx8hf93hs9yggl6njxds6m02g";
    auto toAddress = "bc1qxu5a8gtnjxw3xwdlmr2gl9d76h9fysu3zl656e";
    const int64_t utxoAmount = 342101ll;
    const int64_t toAmount = 300000ll;
    const int64_t byteFee = 126ll;
    Data memo = data("SWAP:THOR.RUNE:thor1tpercamkkxec0q0jk6ltdnlqvsw29guap8wmcl:");

    auto signingInput = Proto::SigningInput();
    signingInput.set_hash_type(TWBitcoinSigHashTypeAll);
    signingInput.set_amount(toAmount);
    signingInput.set_byte_fee(byteFee);
    signingInput.set_to_address(toAddress);
    signingInput.set_change_address(ownAddress);
    signingInput.set_output_op_return(memo.data(), memo.size());

    auto& utxo = *signingInput.add_utxo();
    auto utxoHash = parse_hex("30b82960291a39de3664ec4c844a815e3e680e29b4d3a919e450f0c119cf4e35");
    std::reverse(utxoHash.begin(), utxoHash.end());
    utxo.mutable_out_point()->set_hash(utxoHash.data(), utxoHash.size());
    utxo.mutable_out_point()->set_index(1);
    utxo.mutable_out_point()->set_sequence(UINT32_MAX);
    utxo.set_amount(utxoAmount);

    auto txPlan = TransactionBuilder::plan(signingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {342101}, 300000ull, 205ull * static_cast<Amount>(byteFee)));
    EXPECT_EQ(txPlan.outputOpReturn.size(), 59ull);
    EXPECT_EQ(hex(txPlan.outputOpReturn), "535741503a54484f522e52554e453a74686f72317470657263616d6b6b7865633071306a6b366c74646e6c7176737732396775617038776d636c3a");
    EXPECT_FALSE(txPlan.outputOpReturnIndex.has_value());

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 174ull * static_cast<Amount>(byteFee));
    EXPECT_EQ(feeCalculator.calculate(1, 3, byteFee), 205ull * static_cast<Amount>(byteFee));
}

} // namespace TW::Bitcoin
