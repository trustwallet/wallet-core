// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include "TxComparisonHelper.h"
#include "Bitcoin/InputSelector.h"

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(BitcoinInputSelector, SelectUnspents1) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 11000, 12000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(5000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {11000}));
}

TEST(BitcoinInputSelector, SelectUnspents2) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 50000, 120000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(10000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {50000}));
}

TEST(BitcoinInputSelector, SelectUnspents3) {
    auto utxos = buildTestUTXOs({4000, 2000, 5000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(6000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {4000, 5000}));
}

TEST(BitcoinInputSelector, SelectUnspents4) {
    auto utxos = buildTestUTXOs({40000, 30000, 30000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(50000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {30000, 40000}));
}

TEST(BitcoinInputSelector, SelectUnspents5) {
    auto utxos = buildTestUTXOs({1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(28000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {6000, 7000, 8000, 9000}));
}

TEST(BitcoinInputSelector, SelectUnspents5_simple) {
    auto utxos = buildTestUTXOs({1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(28000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000}));
}

TEST(BitcoinInputSelector, SelectUnspentsInsufficient) {
    auto utxos = buildTestUTXOs({4000, 4000, 4000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(15000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectCustomCase) {
    auto utxos = buildTestUTXOs({794121, 2289357});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(2287189, 61);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {794121, 2289357}));
}

TEST(BitcoinInputSelector, SelectNegativeNoUTXOs) {
    auto utxos = buildTestUTXOs({});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(100000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectNegativeTarget0) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(0, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneTypical) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(50'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));
}

TEST(BitcoinInputSelector, SelectOneInsufficient) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(200'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneInsufficientEqual) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(100'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneInsufficientHigher) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(99'900, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneFitsExactly) {
    auto utxos = buildTestUTXOs({100'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto expectedFee = 174;
    auto selected = selector.select(100'000 - expectedFee, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), expectedFee);
    EXPECT_EQ(feeCalculator.calculate(1, 1, 1), 143);

    // 1 sat more and does not fit any more
    selected = selector.select(100'000 - expectedFee + 1, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectOneFitsExactlyHighfee) {
    auto utxos = buildTestUTXOs({100'000});

    const auto byteFee = 10;
    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto expectedFee = 1740;
    auto selected = selector.select(100'000 - expectedFee, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, byteFee), expectedFee);
    EXPECT_EQ(feeCalculator.calculate(1, 1, byteFee), 1430);

    // 1 sat more and does not fit any more
    selected = selector.select(100'000 - expectedFee + 1, byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectThreeNoDust) {
    auto utxos = buildTestUTXOs({100'000, 70'000, 75'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto selected = selector.select(100'000 - 174 - 10, 1);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    EXPECT_TRUE(verifySelectedUTXOs(selected, {75'000, 100'000}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 174);

    const auto dustLimit = 102;
    // Now 100'000 fits with no dust
    selected = selector.select(100'000 - 174 - dustLimit, 1);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000}));

    // One more and we are over dust limit
    selected = selector.select(100'000 - 174 - dustLimit + 1, 1);
    EXPECT_TRUE(verifySelectedUTXOs(selected, {75'000, 100'000}));
}

TEST(BitcoinInputSelector, SelectTwoFirstEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(15'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {20'000}));
}

TEST(BitcoinInputSelector, SelectTwoSecondEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(70'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {80'000}));
}

TEST(BitcoinInputSelector, SelectTwoBoth) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(90'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {20'000, 80'000}));
}

TEST(BitcoinInputSelector, SelectTwoFirstEnoughButSecond) {
    auto utxos = buildTestUTXOs({20'000, 22'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(18'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {22'000}));
}

TEST(BitcoinInputSelector, SelectTenThree) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5'000, 125'000, 6'000, 150'000, 7'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(300'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000, 125'000, 150'000}));
}

TEST(BitcoinInputSelector, SelectTenThree_simple1) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5'000, 125'000, 6'000, 150'000, 7'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(300'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1'000, 2'000, 100'000, 3'000, 4'000, 5'000, 125'000, 6'000, 150'000}));
}

TEST(BitcoinInputSelector, SelectTenThree_simple2) {
    auto utxos = buildTestUTXOs({150'000, 125'000, 100'000, 7'000, 6'000, 5'000, 4'000, 3'000, 2'000, 1'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(300'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {150'000, 125'000, 100'000}));
}

TEST(BitcoinInputSelector, SelectTenThree_simple3) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 3'000, 4'000, 5'000, 6'000, 7'000, 100'000, 125'000, 150'000});

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(300'000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1'000, 2'000, 3'000, 4'000, 5'000, 6'000, 7'000, 100'000, 125'000, 150'000}));
}

TEST(BitcoinInputSelector, SelectTenThreeExact) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5'000, 125'000, 6'000, 150'000, 7'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    const auto dustLimit = 102;
    auto selected = selector.select(375'000 - 376 - dustLimit, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100'000, 125'000, 150'000}));

    ASSERT_EQ(feeCalculator.calculate(3, 2, 1), 376);

    // one more, and it's too much
    selected = selector.select(375'000 - 376 - dustLimit + 1, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {7'000, 100'000, 125'000, 150'000}));
}

TEST(BitcoinInputSelector, SelectMaxAmountOne) {
    auto utxos = buildTestUTXOs({10189534});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto selected = selector.selectMaxAmount(1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {10189534}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 174);
}

TEST(BitcoinInputSelector, SelectAllAvail) {
    auto utxos = buildTestUTXOs({10189534});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto selected = selector.select(10189534 - 226, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {10189534}));

    EXPECT_EQ(feeCalculator.calculate(1, 2, 1), 174);
}

TEST(BitcoinInputSelector, SelectMaxAmount5of5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto byteFee = 1;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {400, 500, 600, 800, 1000}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 102);
    EXPECT_EQ(feeCalculator.calculate(5, 1, byteFee), 548);
}

TEST(BitcoinInputSelector, SelectMaxAmount4of5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto byteFee = 4;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {500, 600, 800, 1000}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 408);
    EXPECT_EQ(feeCalculator.calculate(4, 1, byteFee), 1784);
}

TEST(BitcoinInputSelector, SelectMaxAmount1of5) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto byteFee = 8;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {1000}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 816);
    EXPECT_EQ(feeCalculator.calculate(1, 1, byteFee), 1144);
}

TEST(BitcoinInputSelector, SelectMaxAmountNone) {
    auto utxos = buildTestUTXOs({400, 500, 600, 800, 1000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto byteFee = 10;
    auto selected = selector.selectMaxAmount(byteFee);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));

    EXPECT_EQ(feeCalculator.calculateSingleInput(byteFee), 1020);
}

TEST(BitcoinInputSelector, SelectMaxAmountNoUTXOs) {
    auto utxos = buildTestUTXOs({});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = InputSelector<UTXO>(utxos, feeCalculator);
    auto selected = selector.selectMaxAmount(1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, SelectZcashUnspents) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(10000, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {73774}));
}

TEST(BitcoinInputSelector, SelectGroestlUnspents) {
    auto utxos = buildTestUTXOs({499971976});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(499951976, 1, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {499971976}));
}

TEST(BitcoinInputSelector, SelectZcashMaxAmount) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.selectMaxAmount(1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {100000, 2592, 73774}));
}

TEST(BitcoinInputSelector, SelectZcashMaxUnspents2) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = InputSelector<UTXO>(utxos, getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(176366 - 6, 1);

    EXPECT_TRUE(verifySelectedUTXOs(selected, {}));
}

TEST(BitcoinInputSelector, ManyUtxos_900) {
    const auto n = 900;
    const auto byteFee = 10;
    std::vector<int64_t> values;
    uint64_t valueSum = 0;
    for (int i = 0; i < n; ++i) {
        const uint64_t val = (i + 1) * 100;
        values.push_back(val);
        valueSum += val;
    }
    const uint64_t requestedAmount = valueSum / 8;
    EXPECT_EQ(requestedAmount, 5'068'125ul);
    auto utxos = buildTestUTXOs(values);

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.select(requestedAmount, byteFee);

    // expected result: 59 utxos, with the largest amounts
    std::vector<int64_t> subset;
    uint64_t subsetSum = 0;
    for (int i = n - 59; i < n; ++i) {
        const uint64_t val = (i + 1) * 100;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 59ul);
    EXPECT_EQ(subsetSum, 5'138'900ul);
    EXPECT_TRUE(verifySelectedUTXOs(selected, subset));
}

TEST(BitcoinInputSelector, ManyUtxos_5000_simple) {
    const auto n = 5000;
    const auto byteFee = 10;
    std::vector<int64_t> values;
    uint64_t valueSum = 0;
    for (int i = 0; i < n; ++i) {
        const uint64_t val = (i + 1) * 100;
        values.push_back(val);
        valueSum += val;
    }
    const uint64_t requestedAmount = valueSum / 20;
    EXPECT_EQ(requestedAmount, 62'512'500ul);
    auto utxos = buildTestUTXOs(values);

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectSimple(requestedAmount, byteFee);

    // expected result: 1205 utxos, with the smaller amounts (except the very small dust ones)
    std::vector<int64_t> subset;
    uint64_t subsetSum = 0;
    for (int i = 10; i < 1205 + 10; ++i) {
        const uint64_t val = (i + 1) * 100;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 1205ul);
    EXPECT_EQ(subsetSum, 73'866'500ul);
    EXPECT_TRUE(verifySelectedUTXOs(selected, subset));
}

TEST(BitcoinInputSelector, ManyUtxos_MaxAmount_5000) {
    const auto n = 5000;
    const auto byteFee = 10;
    std::vector<int64_t> values;
    for (int i = 0; i < n; ++i) {
        const uint64_t val = (i + 1) * 100;
        values.push_back(val);
    }
    auto utxos = buildTestUTXOs(values);

    auto selector = InputSelector<UTXO>(utxos);
    auto selected = selector.selectMaxAmount(byteFee);

    // expected result: 4990 utxos (none of which is dust)
    std::vector<int64_t> subset;
    uint64_t subsetSum = 0;
    for (int i = 10; i < 4990 + 10; ++i) {
        const uint64_t val = (i + 1) * 100;
        subset.push_back(val);
        subsetSum += val;
    }
    EXPECT_EQ(subset.size(), 4990ul);
    EXPECT_EQ(subsetSum, 1'250'244'500ul);
    EXPECT_TRUE(verifySelectedUTXOs(selected, subset));
}

} // namespace TW::Bitcoin
