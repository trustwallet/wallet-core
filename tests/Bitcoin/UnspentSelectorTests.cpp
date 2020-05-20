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

/*

/// Compare a set of selected UTXOs to the expected set of amounts.  Returns "" on match, mismatch description on mismatch.
std::string verifySelectedUTXOs(const std::vector<Proto::UnspentTransaction>& selected, const std::vector<int64_t>& expectedAmounts) {
    std::stringstream ss;
    if (selected.size() != expectedAmounts.size()) {
        ss << "Wrong number of selected UTXOs, " << selected.size() << " vs. " << expectedAmounts.size() << std::endl;
    }
    auto n = expectedAmounts.size();
    for (auto i = 0; i < selected.size() && i < expectedAmounts.size(); ++i) {
        if (expectedAmounts[i] != selected[i].amount()) {
            ss << "Wrong UTXOs amount, pos " << i << " amount " << selected[i].amount() << " expected " << expectedAmounts[i] << std::endl;
        }
    }
    return ss.str();
}
*/

TEST(BitcoinUnspentSelector, SelectUnpsents1) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 11000, 12000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 5000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {11000}), "");
}

TEST(BitcoinUnspentSelector, SelectUnpsents2) {
    auto utxos = buildTestUTXOs({4000, 2000, 6000, 1000, 50000, 120000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {50000}), "");
}

TEST(BitcoinUnspentSelector, SelectUnpsents3) {
    auto utxos = buildTestUTXOs({4000, 2000, 5000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 6000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {4000, 5000}), "");
}

TEST(BitcoinUnspentSelector, SelectUnpsents4) {
    auto utxos = buildTestUTXOs({40000, 30000, 30000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {30000, 40000}), "");
}

TEST(BitcoinUnspentSelector, SelectUnpsents5) {
    auto utxos = buildTestUTXOs({1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 28000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {6000, 7000, 8000, 9000}), "");
}

TEST(BitcoinUnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = buildTestUTXOs({4000, 4000, 4000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectCustomCase) {
    auto utxos = buildTestUTXOs({794121, 2289357});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 2287189, 61);

    EXPECT_EQ(verifySelectedUTXOs(selected, {794121, 2289357}), "");
}

TEST(BitcoinUnspentSelector, SelectNegativeNoUtxo) {
    auto utxos = buildTestUTXOs({});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectNegativeTarget0) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 0, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectOneTypical) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {100'000}), "");
}

TEST(BitcoinUnspentSelector, SelectOneInsufficient) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 200'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectOneInsufficientEqual) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectOneInsufficientHigher) {
    auto utxos = buildTestUTXOs({100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 99'900, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectOneFitsExactly) {
    auto utxos = buildTestUTXOs({100'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 100'000 - 226, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {100'000}), "");

    ASSERT_EQ(feeCalculator.calculate(1, 2, 1), 226);
    ASSERT_EQ(feeCalculator.calculate(1, 1, 1), 192);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - 226 + 1, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectOneFitsExactlyHighfee) {
    auto utxos = buildTestUTXOs({100'000});

    const auto byteFee = 10;
    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 100'000 - 2260, byteFee);

    EXPECT_EQ(verifySelectedUTXOs(selected, {100'000}), "");

    ASSERT_EQ(feeCalculator.calculate(1, 2, byteFee), 2260);
    ASSERT_EQ(feeCalculator.calculate(1, 1, byteFee), 1920);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - 2260 + 1, byteFee);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}

TEST(BitcoinUnspentSelector, SelectThreeNoDust) {
    auto utxos = buildTestUTXOs({100'000, 70'000, 75'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000 - 226 - 10, 1);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    EXPECT_EQ(verifySelectedUTXOs(selected, {75'000, 100'000}), "");
    
    // Now 100'000 fits with no dust; 546 is the dust limit
    selected = selector.select(utxos, 100'000 - 226 - 546, 1);
    EXPECT_EQ(verifySelectedUTXOs(selected, {100'000}), "");

    // One more and we are over dust limit
    selected = selector.select(utxos, 100'000 - 226 - 546 + 1, 1);
    EXPECT_EQ(verifySelectedUTXOs(selected, {75'000, 100'000}), "");
}

TEST(BitcoinUnspentSelector, SelectTwoFirstEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {20'000}), "");
}

TEST(BitcoinUnspentSelector, SelectTwoSecondEnough) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 70'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {80'000}), "");
}

TEST(BitcoinUnspentSelector, SelectTwoBoth) {
    auto utxos = buildTestUTXOs({20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 90'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {20'000, 80'000}), "");
}

TEST(BitcoinUnspentSelector, SelectTwoFirstEnoughButSecond) {
    auto utxos = buildTestUTXOs({20'000, 22'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 18'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {22'000}), "");
}

TEST(BitcoinUnspentSelector, SelectTenThree) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 300'000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {100'000, 125'000, 150'000}), "");
}

TEST(BitcoinUnspentSelector, SelectTenThreeExact) {
    auto utxos = buildTestUTXOs({1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 375'000 - 522 - 546, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {100'000, 125'000, 150'000}), "");

    ASSERT_EQ(feeCalculator.calculate(3, 2, 1), 522);

    // one more, and it's too much
    selected = selector.select(utxos, 375'000 - 522 - 546 + 1, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {7'000, 100'000, 125'000, 150'000}), "");
}

TEST(BitcoinUnspentSelector, SelectMaxCase) {
    auto utxos = buildTestUTXOs({10189534});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 10189534 - 226, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {10189534}), "");

    ASSERT_EQ(feeCalculator.calculate(1, 2, 1), 226);
}

TEST(BitcoinUnspentSelector, SelectZcashUnpsents) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 10000, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {73774}), "");
}

TEST(BitcoinUnspentSelector, SelectGroestlUnpsents) {
    auto utxos = buildTestUTXOs({499971976});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 499951976, 1, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {499971976}), "");
}

TEST(BitcoinUnspentSelector, SelectZcashMaxUnpsents) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 166366, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {2592, 73774, 100000}), "");
}

TEST(BitcoinUnspentSelector, SelectZcashMaxUnpsents2) {
    auto utxos = buildTestUTXOs({100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 176360, 1);

    EXPECT_EQ(verifySelectedUTXOs(selected, {}), "");
}
