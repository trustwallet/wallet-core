// Copyright © 2017-2021 Trust Wallet.
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

const char* ErrorTextNotEnoughUtxos = "Not enough non-dust input UTXOs";

TEST(TransactionPlan, OneTypical) {
    auto utxos = buildTestUTXOs({100'000});
    auto byteFee = 1;
    auto sigingInput = buildSigningInput(50'000, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 50'000, 147));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 226);
}

TEST(TransactionPlan, OneInsufficient) {
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(200'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, OneInsufficientEqual) {
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, OneInsufficientLower100) {
    // requested is only slightly lower than avail, not enough for fee, cannot be satisfied
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000 - 100, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, OneInsufficientLower200) {
    // requested is only slightly lower than avail, enough for fee but too small change, cannot be satisfied
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000 - 200, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, OneInsufficientLower300) {
    auto utxos = buildTestUTXOs({100'000});
    auto sigingInput = buildSigningInput(100'000 - 300, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000 - 300, 147));
}

TEST(TransactionPlan, OneFitsExactly) {
    auto utxos = buildTestUTXOs({100'000});
    auto byteFee = 1;
    auto expectedFee = 147;
    auto sigingInput = buildSigningInput(100'000 - 226, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000 - 226, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 226);
}

TEST(TransactionPlan, OneFitsExactlyHighFee) {
    auto utxos = buildTestUTXOs({100'000});
    auto byteFee = 10;
    auto expectedFee = 1470;
    auto sigingInput = buildSigningInput(100'000 - 2260, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000 - 2260, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), 2260);
}

TEST(TransactionPlan, TwoFirstEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});
    auto sigingInput = buildSigningInput(15'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {20'000}, 15'000, 147));
}

TEST(TransactionPlan, TwoSecondEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});
    auto sigingInput = buildSigningInput(70'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {80'000}, 70'000, 147));
}

TEST(TransactionPlan, TwoBoth) {
    auto utxos = buildTestUTXOs({20'000, 80'000});
    auto sigingInput = buildSigningInput(90'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {20'000, 80'000}, 90'000, 215));
}

TEST(TransactionPlan, TwoFirstEnoughButSecond) {
    auto utxos = buildTestUTXOs({20'000, 22'000});
    auto sigingInput = buildSigningInput(18'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {22'000}, 18'000, 147));
}

TEST(TransactionPlan, ThreeNoDust) {
    auto utxos = buildTestUTXOs({100'000, 70'000, 75'000});
    auto sigingInput = buildSigningInput(100'000 - 226 - 10, 1, utxos);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {75'000, 100'000}, 100'000 - 226 - 10, 215));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 226);
    EXPECT_EQ(feeCalculator.calculate(2, 2, 1), 374);

    // Now 100'000 fits with no dust; 546 is the dust limit
    sigingInput = buildSigningInput(100'000 - 226 - 546, 1, utxos);
    txPlan = TransactionBuilder::plan(sigingInput);
    EXPECT_TRUE(verifyPlan(txPlan, {100'000}, 100'000 - 226 - 546, 147));

    // One more and we are over dust limit
    sigingInput = buildSigningInput(100'000 - 226 - 546 + 1, 1, utxos);
    txPlan = TransactionBuilder::plan(sigingInput);
    EXPECT_TRUE(verifyPlan(txPlan, {75'000, 100'000}, 100'000 - 226 - 546 + 1, 215));
}

TEST(TransactionPlan, TenThree) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});
    auto sigingInput = buildSigningInput(300'000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100'000, 125'000, 150'000}, 300'000, 283));
}

TEST(TransactionPlan, NonMaxAmount) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 50000, 120000});
    auto sigingInput = buildSigningInput(10000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {50000}, 10000, 147));
}

TEST(TransactionPlan, UnspentsInsufficient) {
    auto utxos = buildTestUTXOs({4000, 4000, 4000});
    auto sigingInput = buildSigningInput(15000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, SelectionSuboptimal_ExtraSmallUtxo) {
    // Solution found 5-in-2-out {400, 500, 600, 800, 1000} avail 3300 txamount 1570 fee 838 change 892
    // The 400 UTXO is smaller than the change, smaller than half the change -- could be omitted
    // Better solution: 3-in-2-out {600, 800, 1000} avail 2400 txamount 1570 fee 566 change 264
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1'000});
    auto byteFee = 2;
    auto sigingInput = buildSigningInput(1'570, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 838;
    EXPECT_TRUE(verifyPlan(txPlan, {400, 500, 600, 800, 1'000}, 1'570, expectedFee));
    auto change = 3'300 - 1'570 - expectedFee;
    auto firstUtxo = txPlan.utxos[0].amount();
    EXPECT_TRUE(change / 2 > txPlan.utxos[0].amount());
    EXPECT_EQ(change, 892);
    EXPECT_EQ(firstUtxo, 400);
}

TEST(TransactionPlan, SelectionFail_CouldBeSatisfied5) {
    // 5-input case, where no 1,2,3,4,or 5 UTXO input is found.
    // A solution exists though, actual fee is lower than estimate used in selection.
    // Solutions not found are:
    //  4-in-2-out {500, 600, 800, 1000} avail 2900 txamount 1775 fee 702 change 423
    //  3-in-2-out {600, 800, 1'000}     avail 2400 txamount 1775 fee 566 change 59
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1'000});
    auto byteFee = 2;
    auto sigingInput = buildSigningInput(1'775, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, Inputs5_33Req19NoDustFee2) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    auto byteFee = 2;
    auto sigingInput = buildSigningInput(19'000, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 283*byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 19'000, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 296);
}

TEST(TransactionPlan, Inputs5_33Req19Dust1Fee5) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    auto byteFee = 5;
    auto sigingInput = buildSigningInput(19'000, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 283*byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 19'000, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 740);
}

TEST(TransactionPlan, Inputs5_33Req19Dust1Fee9) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    auto byteFee = 9;
    auto sigingInput = buildSigningInput(19'000, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 283*byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 19'000, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 1332);
}

TEST(TransactionPlan, Inputs5_33Req19Fee20) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    auto byteFee = 20;
    auto sigingInput = buildSigningInput(19'000, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, Inputs5_33Req13Fee20) {
    auto utxos = buildTestUTXOs({600, 1'200, 6'000, 8'000, 10'000});
    auto byteFee = 20;
    auto sigingInput = buildSigningInput(13'000, byteFee, utxos);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 283*byteFee;
    EXPECT_TRUE(verifyPlan(txPlan, {6'000, 8'000, 10'000}, 13'000, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 2960);
}

TEST(TransactionPlan, NoUTXOs) {
    auto utxos = buildTestUTXOs({});
    auto sigingInput = buildSigningInput(15000, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, "Missing input UTXOs"));
}

TEST(TransactionPlan, CustomCase) {
    auto utxos = buildTestUTXOs({794121, 2289357});
    auto byteFee = 61;
    auto sigingInput = buildSigningInput(2287189, byteFee, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {794121, 2289357}, 2287189, 13115));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(2, 2, byteFee), 22814);
}

TEST(TransactionPlan, Target0) {
    auto utxos = buildTestUTXOs({2000, 3000});
    auto sigingInput = buildSigningInput(0, 1, utxos);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, "Zero amount requested"));
}

TEST(TransactionPlan, MaxAmount) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200);
    auto byteFee = 32;
    auto sigingInput = buildSigningInput(39200, byteFee, utxos, true);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4736);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 5792;
    EXPECT_TRUE(verifyPlan(txPlan, {15000, 15000}, 30000 - expectedFee, expectedFee));
}

TEST(TransactionPlan, MaxAmountOne) {
    auto utxos = buildTestUTXOs({10189534});
    auto sigingInput = buildSigningInput(100, 1, utxos, true);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 113;
    EXPECT_TRUE(verifyPlan(txPlan, {10189534}, 10189534 - expectedFee, expectedFee));
}

TEST(TransactionPlan, AmountEqualsMaxButNotUseMax) {
    // amount is set to max, but UseMax is not set --> cannot be satisfied
    auto utxos = buildTestUTXOs({10189534});
    auto sigingInput = buildSigningInput(10189534, 1, utxos, false);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));
}

TEST(TransactionPlan, MaxAmountLowerRequested) {
    auto utxos = buildTestUTXOs({4000, 2000, 15000, 15000, 3000, 200});
    ASSERT_EQ(sumUTXOs(utxos), 39200);
    auto byteFee = 32;
    auto sigingInput = buildSigningInput(10, byteFee, utxos, true);

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 4736);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 5792;
    EXPECT_TRUE(verifyPlan(txPlan, {15000, 15000}, 30000 - expectedFee, expectedFee));
}

TEST(TransactionPlan, MaxAmountNoDustFee2) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    auto byteFee = 2;
    auto sigingInput = buildSigningInput(100, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 770;
    EXPECT_TRUE(verifyPlan(txPlan, {400, 500, 600, 800, 1000}, 3'300 - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 296);
    EXPECT_EQ(feeCalculator.calculate(5, 1, byteFee), 1568);
}

TEST(TransactionPlan, MaxAmountDust1Fee3) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    auto byteFee = 3;
    auto sigingInput = buildSigningInput(100, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 951;
    EXPECT_TRUE(verifyPlan(txPlan, {500, 600, 800, 1000}, 2'900 - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 444);
    EXPECT_EQ(feeCalculator.calculate(4, 1, byteFee), 1908);
}

TEST(TransactionPlan, MaxAmountDust2Fee4) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    auto byteFee = 4;
    auto sigingInput = buildSigningInput(100, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    auto expectedFee = 996;
    EXPECT_TRUE(verifyPlan(txPlan, {600, 800, 1000}, 2'400 - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 592);
    EXPECT_EQ(feeCalculator.calculate(3, 1, byteFee), 1952);
}

TEST(TransactionPlan, MaxAmountDustAllFee7) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});
    auto byteFee = 7;
    auto sigingInput = buildSigningInput(100, byteFee, utxos, true);

    // UTXOs smaller than singleInputFee are not included
    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {}, 0, 0, ErrorTextNotEnoughUtxos));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 1036);
}

TEST(TransactionPlan, One_MaxAmount_FeeMoreThanAvailable) {
    auto utxos = buildTestUTXOs({170});
    auto byteFee = 1;
    auto expectedFee = 113;
    auto sigingInput = buildSigningInput(300, byteFee, utxos, true);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    // Fee is reduced to availableAmount
    EXPECT_TRUE(verifyPlan(txPlan, {170}, 170 - expectedFee, expectedFee));

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    EXPECT_EQ(feeCalculator.calculate(1, 1, byteFee), 192);
}

TEST(TransactionPlan, MaxAmountDoge) {
    auto utxos = buildTestUTXOs({Amount(100000000), Amount(2000000000), Amount(200000000)});
    ASSERT_EQ(sumUTXOs(utxos), Amount(2300000000));
    auto sigingInput = buildSigningInput(Amount(2300000000), 100, utxos, true, TWCoinTypeDogecoin);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {100000000, 2000000000, 200000000}, 2299951200, 48800));
}

TEST(TransactionPlan, AmountDecred) {
    auto utxos = buildTestUTXOs({Amount(39900000)});
    auto sigingInput = buildSigningInput(Amount(10000000), 10, utxos, false, TWCoinTypeDecred);

    auto txPlan = TransactionBuilder::plan(sigingInput);

    EXPECT_TRUE(verifyPlan(txPlan, {39900000}, 10000000, 2540));
}
