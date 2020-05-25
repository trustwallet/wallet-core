// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include "TxComparisonHelper.h"
#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/UnspentSelector.h"
#include "proto/Bitcoin.pb.h"

#include <gtest/gtest.h>
#include <sstream>

using namespace TW;
using namespace TW::Bitcoin;

TEST(BitcoinUnspentSelector, SelectUnpsents1) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 11000, 12000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 5000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {11000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents2) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 50000, 120000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {50000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents3) {
    auto utxos = buildTestUTXOs({4000, 2000, 5000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 6000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {4000, 5000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents4) {
    auto utxos = buildTestUTXOs({40000, 30000, 30000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {30000, 40000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents5) {
    auto utxos = buildTestUTXOs({1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 28000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {6000, 7000, 8000, 9000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = buildTestUTXOs({4000, 4000, 4000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectCustomCase) {
    auto utxos = buildTestUTXOs({794121, 2289357});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 2287189, 61);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {794121, 2289357}));
}

TEST(BitcoinUnspentSelector, SelectNegativeNoUTXOs) {
    auto utxos = buildTestUTXOs({});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectNegativeTarget0) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 0, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneTypical) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));
}

TEST(BitcoinUnspentSelector, SelectOneInsufficient) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 200'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneInsufficientEqual) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneInsufficientHigher) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 99'900, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneFitsExactly) {
    auto utxos = buildTestUTXOs({100'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto expectedFee = 226;
    auto selected = selector.select(utxos, 100'000 - expectedFee, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));

    ASSERT_EQ(feeCalculator.calculate(1, 2, 1), expectedFee);
    ASSERT_EQ(feeCalculator.calculate(1, 1, 1), 192);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - expectedFee + 1, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneFitsExactlyHighfee) {
    auto utxos = buildTestUTXOs({100'000});

    const auto byteFee = 10;
    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto expectedFee = 2260;
    auto selected = selector.select(utxos, 100'000 - expectedFee, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));

    ASSERT_EQ(feeCalculator.calculate(1, 2, byteFee), expectedFee);
    ASSERT_EQ(feeCalculator.calculate(1, 1, byteFee), 1920);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - expectedFee + 1, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectThreeNoDust) {
    auto utxos = buildTestUTXOs({100'000, 70'000, 75'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000 - 226 - 10, 1);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    EXPECT_TRUE(verifySelectedUTXOs(selected, {75'000, 100'000}));
    
    // Now 100'000 fits with no dust; 546 is the dust limit
    selected = selector.select(utxos, 100'000 - 226 - 546, 1);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));

    // One more and we are over dust limit
    selected = selector.select(utxos, 100'000 - 226 - 546 + 1, 1);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {75'000, 100'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoFirstEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {20'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoSecondEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 70'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {80'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoBoth) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 90'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {20'000, 80'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoFirstEnoughButSecond) {
    auto utxos = buildTestUTXOs({20'000, 22'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 18'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {22'000}));
}

TEST(BitcoinUnspentSelector, SelectTenThree) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 300'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000, 125'000, 150'000}));
}

TEST(BitcoinUnspentSelector, SelectTenThreeExact) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 375'000 - 522 - 546, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000, 125'000, 150'000}));

    ASSERT_EQ(feeCalculator.calculate(3, 2, 1), 522);

    // one more, and it's too much
    selected = selector.select(utxos, 375'000 - 522 - 546 + 1, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {7'000, 100'000, 125'000, 150'000}));
}

TEST(BitcoinUnspentSelector, SelectMaxAmountOne) {
    auto utxos = buildTestUTXOs({10189534});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.selectMaxAmount(utxos, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {10189534}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 226);
}

TEST(BitcoinUnspentSelector, SelectAllAvail) {
    auto utxos = buildTestUTXOs({10189534});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 10189534 - 226, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {10189534}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 226);
}

TEST(BitcoinUnspentSelector, SelectMaxAmount5of5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto byteFee = 1;
    auto selected = selector.selectMaxAmount(utxos, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {400, 500, 600, 800, 1000}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 148);
    EXPECT_EQ(feeCalculator.calculate(5, 1, byteFee), 784);
}

TEST(BitcoinUnspentSelector, SelectMaxAmount4of5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto byteFee = 3;
    auto selected = selector.selectMaxAmount(utxos, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {500, 600, 800, 1000}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 444);
    EXPECT_EQ(feeCalculator.calculate(4, 1, byteFee), 1908);
}

TEST(BitcoinUnspentSelector, SelectMaxAmount1of5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto byteFee = 6;
    auto selected = selector.selectMaxAmount(utxos, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1000}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 888);
    EXPECT_EQ(feeCalculator.calculate(1, 1, byteFee), 1152);
}

TEST(BitcoinUnspentSelector, SelectMaxAmountNone) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto byteFee = 10;
    auto selected = selector.selectMaxAmount(utxos, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 1480);
}

TEST(BitcoinUnspentSelector, SelectMaxAmountNoUTXOs) {
    auto utxos = buildTestUTXOs({});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.selectMaxAmount(utxos, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectZcashUnpsents) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 10000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {73774}));
}

TEST(BitcoinUnspentSelector, SelectGroestlUnpsents) {
    auto utxos = buildTestUTXOs({499971976});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 499951976, 1, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {499971976}));
}

TEST(BitcoinUnspentSelector, SelectZcashMaxAmount) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.selectMaxAmount(utxos, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100000, 2592, 73774}));
}

TEST(BitcoinUnspentSelector, SelectZcashMaxUnpsents2) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 176366 - 6, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}
