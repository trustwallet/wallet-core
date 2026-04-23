// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWCoinType.h>

#include "TxComparisonHelper.h"
#include "Bitcoin/InputSelector.h"

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(BitcoinInputSelector, SelectUnspents1) {
    auto utxos = buildTestUTXOs({4000ull, 2000ull, 6000ull, 1000ull, 11000ull, 12000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(5000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {11000ull}));
}

TEST(BitcoinInputSelector, SelectUnspents2) {
    auto utxos = buildTestUTXOs({4000ull, 2000ull, 6000ull, 1000ull, 50000ull, 120000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(10000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {50000ull}));
}

TEST(BitcoinInputSelector, SelectUnspents3) {
    auto utxos = buildTestUTXOs({4000ull, 2000ull, 5000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(6000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {4000ull, 5000ull}));
}

TEST(BitcoinInputSelector, SelectUnspents4) {
    auto utxos = buildTestUTXOs({40000ull, 30000ull, 30000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(50000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {30000ull, 40000ull}));
}

TEST(BitcoinInputSelector, SelectUnspents5) {
    auto utxos = buildTestUTXOs({1000ull, 2000ull, 3000ull, 4000ull, 5000ull, 6000ull, 7000ull, 8000ull, 9000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(28000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {6000ull, 7000ull, 8000ull, 9000ull}));
}

TEST(BitcoinInputSelector, SelectUnspents5_simple) {
    auto utxos = buildTestUTXOs({1000ull, 2000ull, 3000ull, 4000ull, 5000ull, 6000ull, 7000ull, 8000ull, 9000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(28000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1000ull, 2000ull, 3000ull, 4000ull, 5000ull, 6000ull, 7000ull, 8000ull}));
}

TEST(BitcoinInputSelector, SelectUnspentsInsufficient) {
    auto utxos = buildTestUTXOs({4000ull, 4000ull, 4000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(15000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectCustomCase) {
    auto utxos = buildTestUTXOs({794121ull, 2289357ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(2287189ull, 61ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {794121ull, 2289357ull}));
}

TEST(BitcoinInputSelector, SelectNegativeNoUTXOs) {
    auto utxos = buildTestUTXOs({});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(100000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectNegativeTarget0) {
    auto utxos = buildTestUTXOs({100'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(0ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneTypical) {
    auto utxos = buildTestUTXOs({100'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(50'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));
}

TEST(BitcoinInputSelector, SelectOneInsufficient) {
    auto utxos = buildTestUTXOs({100'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(200'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneInsufficientEqual) {
    auto utxos = buildTestUTXOs({100'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(100'000ull, 1ull);

    // `InputSelector` returns the entire list of UTXOs even if they are not enough.
    // That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));
}

TEST(BitcoinInputSelector, SelectOneInsufficientHigher) {
    auto utxos = buildTestUTXOs({100'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(99'900ull, 1ull);

    // `InputSelector` returns the entire list of UTXOs even if they are not enough.
    // That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));
}

TEST(BitcoinInputSelector, SelectOneInsufficientHigherFilterDust) {
    auto utxos = buildTestUTXOs({22ull, 100'000ull, 40ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(99'900ull, 1ull);

    // `InputSelector` returns the entire list of UTXOs even if they are not enough.
    // That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    // However, the list of result UTXOs does not include dust inputs.
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));
}

TEST(BitcoinInputSelector, SelectOneFitsExactly) {
    auto utxos = buildTestUTXOs({100'000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount expectedFee = 174ull;
    auto selected = selector.select(100'000ull - expectedFee, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));

    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), expectedFee);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, 1ull), 143ull);

    // 1 sat more and does not fit any more.
    // However, `InputSelector` returns the entire list of UTXOs even if they are not enough.
    // That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    selected = selector.select(100'000ull - expectedFee + 1ull, 1ull);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));
}

TEST(BitcoinInputSelector, SelectOneFitsExactlyHighfee) {
    auto utxos = buildTestUTXOs({100'000ull});

    const Amount byteFee = 10ull;
    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount expectedFee = 1740ull;
    auto selected = selector.select(100'000ull - expectedFee, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));

    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, byteFee), expectedFee);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, byteFee), 1430ull);

    // 1 sat more and does not fit any more.
    // However, `InputSelector` returns the entire list of UTXOs even if they are not enough.
    // That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    selected = selector.select(100'000ull - expectedFee + 1ull, byteFee);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));
}

TEST(BitcoinInputSelector, SelectThreeNoDust) {
    auto utxos = buildTestUTXOs({100'000ull, 70'000ull, 75'000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(100'000ull - 174ull - 10ull, 1ull);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    EXPECT_TRUE(verifySelectedUTXOs(selected, {75'000ull, 100'000ull}));

    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 174ull);

    const Amount dustLimit = 102ull;
    // Now 100'000 fits with no dust
    selected = selector.select(100'000ull - 174ull - dustLimit, 1ull);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull}));

    // One more and we are over dust limit
    selected = selector.select(100'000ull - 174ull - dustLimit + 1ull, 1ull);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {75'000ull, 100'000ull}));
}

TEST(BitcoinInputSelector, SelectTwoFirstEnough) {
    auto utxos = buildTestUTXOs({20'000ull, 80'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(15'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {20'000ull}));
}

TEST(BitcoinInputSelector, SelectTwoSecondEnough) {
    auto utxos = buildTestUTXOs({20'000ull, 80'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(70'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {80'000ull}));
}

TEST(BitcoinInputSelector, SelectTwoBoth) {
    auto utxos = buildTestUTXOs({20'000ull, 80'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(90'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {20'000ull, 80'000ull}));
}

TEST(BitcoinInputSelector, SelectTwoFirstEnoughButSecond) {
    auto utxos = buildTestUTXOs({20'000ull, 22'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(18'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {22'000ull}));
}

TEST(BitcoinInputSelector, SelectTenThree) {
    auto utxos = buildTestUTXOs({1'000ull, 2'000ull, 100'000ull, 3'000ull, 4'000ull, 5'000ull, 125'000ull, 6'000ull, 150'000ull, 7'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(300'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull, 125'000ull, 150'000ull}));
}

TEST(BitcoinInputSelector, SelectTenThree_simple1) {
    auto utxos = buildTestUTXOs({1'000ull, 2'000ull, 100'000ull, 3'000ull, 4'000ull, 5'000ull, 125'000ull, 6'000ull, 150'000ull, 7'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(300'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1'000ull, 2'000ull, 100'000ull, 3'000ull, 4'000ull, 5'000ull, 125'000ull, 6'000ull, 150'000ull}));
}

TEST(BitcoinInputSelector, SelectTenThree_simple2) {
    auto utxos = buildTestUTXOs({150'000ull, 125'000ull, 100'000ull, 7'000ull, 6'000ull, 5'000ull, 4'000ull, 3'000ull, 2'000ull, 1'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(300'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {150'000ull, 125'000ull, 100'000ull}));
}

TEST(BitcoinInputSelector, SelectTenThree_simple3) {
    auto utxos = buildTestUTXOs({1'000ull, 2'000ull, 3'000ull, 4'000ull, 5'000ull, 6'000ull, 7'000ull, 100'000ull, 125'000ull, 150'000ull});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(300'000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1'000ull, 2'000ull, 3'000ull, 4'000ull, 5'000ull, 6'000ull, 7'000ull, 100'000ull, 125'000ull, 150'000ull}));
}

TEST(BitcoinInputSelector, SelectTenThreeExact) {
    auto utxos = buildTestUTXOs({1'000ull, 2'000ull, 100'000ull, 3'000ull, 4'000ull, 5'000ull, 125'000ull, 6'000ull, 150'000ull, 7'000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount dustLimit = 102ull;
    auto selected = selector.select(375'000ull - 376ull - dustLimit, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000ull, 125'000ull, 150'000ull}));

    ASSERT_EQ(feeCalculator.calculate(3ull, 2ull, 1ull), 376ull);

    // one more, and it's too much
    selected = selector.select(375'000ull - 376ull - dustLimit + 1ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {7'000ull, 100'000ull, 125'000ull, 150'000ull}));
}

TEST(BitcoinInputSelector, SelectMaxAmountOne) {
    auto utxos = buildTestUTXOs({10189534ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectMaxAmount(1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {10189534ull}));

    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 174ull);
}

TEST(BitcoinInputSelector, SelectAllAvail) {
    auto utxos = buildTestUTXOs({10189534ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(10189534ull - 226ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {10189534ull}));

    EXPECT_EQ(feeCalculator.calculate(1ull, 2ull, 1ull), 174ull);
}

TEST(BitcoinInputSelector, SelectMaxAmount5of5) {
    auto utxos = buildTestUTXOs({400ull, 500ull, 600ull, 800ull, 1000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount byteFee = 1ull;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {400ull, 500ull, 600ull, 800ull, 1000ull}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 102ull);
    EXPECT_EQ(feeCalculator.calculate(5ull, 1ull, byteFee), 548ull);
}

TEST(BitcoinInputSelector, SelectMaxAmount4of5) {
    auto utxos = buildTestUTXOs({400ull, 500ull, 600ull, 800ull, 1000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount byteFee = 4ull;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {500ull, 600ull, 800ull, 1000ull}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 408ull);
    EXPECT_EQ(feeCalculator.calculate(4ull, 1ull, byteFee), 1784ull);
}

TEST(BitcoinInputSelector, SelectMaxAmount1of5) {
    auto utxos = buildTestUTXOs({400ull, 500ull, 600ull, 800ull, 1000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount byteFee = 8ull;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1000ull}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 816ull);
    EXPECT_EQ(feeCalculator.calculate(1ull, 1ull, byteFee), 1144ull);
}

TEST(BitcoinInputSelector, SelectMaxAmountNone) {
    auto utxos = buildTestUTXOs({400ull, 500ull, 600ull, 800ull, 1000ull});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos);
    const Amount byteFee = 10ull;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 1020ull);
}

TEST(BitcoinInputSelector, SelectMaxAmountNoUTXOs) {
    auto utxos = buildTestUTXOs({});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectMaxAmount(1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectZcashUnspents) {
    auto utxos = buildTestUTXOs({100000ull, 2592ull, 73774ull});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash), std::make_shared<LegacyDustCalculator>(TWCoinTypeZcash));
    auto selected = selector.select(10000ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {73774ull}));
}

TEST(BitcoinInputSelector, SelectGroestlUnspents) {
    auto utxos = buildTestUTXOs({499971976ull});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash), std::make_shared<LegacyDustCalculator>(TWCoinTypeZcash));
    auto selected = selector.select(499951976ull, 1ull, 1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {499971976ull}));
}

TEST(BitcoinInputSelector, SelectZcashMaxAmount) {
    auto utxos = buildTestUTXOs({100000ull, 2592ull, 73774ull});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash), std::make_shared<LegacyDustCalculator>(TWCoinTypeZcash));
    auto selected = selector.selectMaxAmount(1ull);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100000ull, 2592ull, 73774ull}));
}

TEST(BitcoinInputSelector, SelectZcashMaxUnspents2) {
    auto utxos = buildTestUTXOs({100000ull, 2592ull, 73774ull});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash), std::make_shared<LegacyDustCalculator>(TWCoinTypeZcash));
    auto selected = selector.select(176366ull - 6ull, 1ull);

    // `InputSelector` returns the entire list of UTXOs even if they are not enough.
    // That's because `InputSelector` has a rough segwit fee estimation algorithm, and the UTXOs can actually be enough.
    EXPECT_TRUE(verifySelectedUTXOs(selected, {2592ull, 73774ull, 100000ull}));
}

TEST(BitcoinInputSelector, ManyUtxos_900) {
    const auto n = 900;
    const Amount byteFee = 10ull;
    std::vector<Amount> values;
    Amount valueSum = 0ull;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        values.push_back(val);
        valueSum += val;
    }
    const Amount requestedAmount = valueSum / 8ull;
    EXPECT_EQ(requestedAmount, 5'068'125ull);
    auto utxos = buildTestUTXOs(values);

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(requestedAmount, byteFee);

    // expected result: 59 utxos, with the largest amounts
    std::vector<Amount> subset;
    Amount subsetSum = 0ull;
    for (int i = n - 59; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 59ull);
    EXPECT_EQ(subsetSum, 5'138'900ull);
    EXPECT_TRUE(verifySelectedUTXOs(selected, subset));
}

TEST(BitcoinInputSelector, ManyUtxos_5000_simple) {
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
    auto utxos = buildTestUTXOs(values);

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(requestedAmount, byteFee);

    // expected result: 1205 utxos, with the smaller amounts (except the very small dust ones)
    std::vector<Amount> subset;
    Amount subsetSum = 0ull;
    for (int i = 10; i < 1205 + 10; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 1205ull);
    EXPECT_EQ(subsetSum, 73'866'500ull);
    EXPECT_TRUE(verifySelectedUTXOs(selected, subset));
}

TEST(BitcoinInputSelector, ManyUtxos_MaxAmount_5000) {
    const auto n = 5000;
    const Amount byteFee = 10ull;
    std::vector<Amount> values;
    for (int i = 0; i < n; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        values.push_back(val);
    }
    auto utxos = buildTestUTXOs(values);

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectMaxAmount(byteFee);

    // expected result: 4990 utxos (none of which is dust)
    std::vector<Amount> subset;
    Amount subsetSum = 0ull;
    for (int i = 10; i < 4990 + 10; ++i) {
        const Amount val = static_cast<Amount>(i + 1) * 100ull;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 4990ull);
    EXPECT_EQ(subsetSum, 1'250'244'500ull);
    EXPECT_TRUE(verifySelectedUTXOs(selected, subset));
}

} // namespace TW::Bitcoin
