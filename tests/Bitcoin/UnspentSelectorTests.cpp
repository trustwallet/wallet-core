// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/UnspentSelector.h"
#include "proto/Bitcoin.pb.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

auto transactionOutPoint = OutPoint(std::vector<uint8_t>(32), 0);

inline auto sum(const std::vector<Proto::UnspentTransaction>& utxos) {
    int64_t s = 0u;
    for (auto& utxo : utxos) {
        s += utxo.amount();
    }
    return s;
}

inline auto buildUTXO(int64_t amount) {
    Proto::UnspentTransaction utxo;
    utxo.set_amount(amount);
    const auto& outPoint = transactionOutPoint;
    utxo.mutable_out_point()->set_hash(outPoint.hash.data(), outPoint.hash.size());
    utxo.mutable_out_point()->set_index(outPoint.index);
    return utxo;
}

void buildUTXOs(std::vector<Proto::UnspentTransaction>& utxos, const std::vector<int64_t>& amounts) {
    for (auto it = amounts.begin(); it != amounts.end(); it++) {
        utxos.push_back(buildUTXO(*it));
    }
}

bool verifySelected(const std::vector<Proto::UnspentTransaction>& selected, const std::vector<int64_t>& expectedAmounts) {
    if (selected.size() != expectedAmounts.size()) {
        std::cout << "Wrong number of selected UTXOs, " << selected.size() << " vs. " << expectedAmounts.size() << std::endl;
        return false;
    }
    auto n = expectedAmounts.size();
    for (auto i = 0; i < n; ++i) {
        if (expectedAmounts[i] != selected[i].amount()) {
            std::cout << "Wrong UTXOs amount, pos " << i << " amount " << selected[i].amount() << " expected " << expectedAmounts[i] << std::endl;
            return false;
        }
    }
    return true;
}

TEST(UnspentSelector, SelectUnpsents1) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 2000, 6000, 1000, 11000, 12000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 5000, 1);

    ASSERT_TRUE(verifySelected(selected, {11000}));
}

TEST(UnspentSelector, SelectUnpsents2) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 2000, 6000, 1000, 50000, 120000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10000, 1);

    ASSERT_TRUE(verifySelected(selected, {50000}));
}

TEST(UnspentSelector, SelectUnpsents3) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 2000, 5000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 6000, 1);

    ASSERT_TRUE(verifySelected(selected, {4000, 5000}));
}

TEST(UnspentSelector, SelectUnpsents4) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {40000, 30000, 30000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50000, 1);

    ASSERT_EQ(sum(selected), 70000);
}

TEST(UnspentSelector, SelectUnpsents5) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 28000, 1);

    ASSERT_EQ(sum(selected), 30000);
}

TEST(UnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 4000, 4000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15000, 1);

    ASSERT_TRUE(selected.empty());
}

TEST(UnspentSelector, SelectCustomCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {794121, 2289357});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 2287189, 61);

    ASSERT_EQ(sum(selected), 3083478);
}

TEST(UnspentSelector, SelectNegativeNoUtxo) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100000, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(UnspentSelector, SelectNegativeTarget0) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 0, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(UnspentSelector, SelectOneTypical) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50'000, 1);

    ASSERT_TRUE(verifySelected(selected, {100'000}));
}

TEST(UnspentSelector, SelectOneInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 200'000, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(UnspentSelector, SelectOneInsufficientEqual) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(UnspentSelector, SelectOneInsufficientHigher) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 99'900, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(UnspentSelector, SelectOneFitsExactly) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000 - 192, 1); // shuold be 226

    ASSERT_TRUE(verifySelected(selected, {100'000}));

    ASSERT_EQ(selector.calculator.calculate(1, 2, 1), 226);
    ASSERT_EQ(selector.calculator.calculate(1, 1, 1), 192);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - 192 + 1, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(UnspentSelector, SelectThreeNoDust) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000, 70'000, 75'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000 - 226 - 10, 1);

    // 100'000 would fit with dust; instead two UTXOs are selected not to leave dust
    ASSERT_TRUE(verifySelected(selected, {75'000, 100'000}));
    
    // Now 100'000 fits with no dust; 546 is the dust limit
    selected = selector.select(utxos, 100'000 - 226 - 546, 1);
    ASSERT_TRUE(verifySelected(selected, {100'000}));

    // One more and we are over dust limit
    selected = selector.select(utxos, 100'000 - 226 - 546 + 1, 1);
    ASSERT_TRUE(verifySelected(selected, {75'000, 100'000}));
}

TEST(UnspentSelector, SelectTwoFirstEnough) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15'000, 1);

    ASSERT_TRUE(verifySelected(selected, {20'000}));
}

TEST(UnspentSelector, SelectTwoSecondEnough) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 70'000, 1);

    ASSERT_TRUE(verifySelected(selected, {80'000}));
}

TEST(UnspentSelector, SelectTwoBoth) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 90'000, 1);

    ASSERT_TRUE(verifySelected(selected, {20'000, 80'000}));
}

TEST(UnspentSelector, SelectTwoFirstEnoughButSecond) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 22'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 18'000, 1);

    ASSERT_TRUE(verifySelected(selected, {22'000}));
}

TEST(UnspentSelector, SelectTenThree) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 300'000, 1);

    ASSERT_TRUE(verifySelected(selected, {100'000, 125'000, 150'000}));
}

TEST(UnspentSelector, SelectTenThreeExact) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 375'000 - 522 - 546, 1);

    ASSERT_TRUE(verifySelected(selected, {100'000, 125'000, 150'000}));

    auto fee = selector.calculator.calculate(3, 2, 1);
    ASSERT_EQ(fee, 522);

    // one more, and it's too much
    selected = selector.select(utxos, 375'000 - 522 - 546 + 1, 1);

    ASSERT_TRUE(verifySelected(selected, {7'000, 100'000, 125'000, 150'000}));
}

TEST(UnspentSelector, SelectMaxCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {10189534});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10189534 - 226, 1);

    ASSERT_TRUE(verifySelected(selected, {10189534}));

    auto fee = selector.calculator.calculate(1, 2, 1);
    ASSERT_EQ(fee, 226);
}

TEST(UnspentSelector, SelectZcashUnpsents) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100000, 2592, 73774});

    auto calculator = UnspentCalculator::getCalculator(TWCoinTypeZcash);
    auto selector = UnspentSelector(calculator);
    auto selected = selector.select(utxos, 10000, 1);

    ASSERT_EQ(sum(selected), 73774);
    ASSERT_TRUE(selected.size() > 0);
}

TEST(UnspentSelector, SelectGroestlUnpsents) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {499971976});

    auto calculator = UnspentCalculator::getCalculator(TWCoinTypeGroestlcoin);
    auto selector = UnspentSelector(calculator);
    auto selected = selector.select(utxos, 499951976, 1, 1);

    ASSERT_EQ(sum(selected), 499971976);
    ASSERT_TRUE(selected.size() > 0);
}

TEST(UnspentSelector, SelectZcashMaxUnpsents) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100000, 2592, 73774});

    auto calculator = UnspentCalculator::getCalculator(TWCoinTypeZcash);
    auto selector = UnspentSelector(calculator);
    auto selected = selector.select(utxos, 166366, 1);

    ASSERT_EQ(sum(selected), 176366);
    ASSERT_TRUE(selected.size() > 0);
}

TEST(UnspentSelector, SelectZcashMaxUnpsents2) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100000, 2592, 73774});

    auto calculator = UnspentCalculator::getCalculator(TWCoinTypeZcash);
    auto selector = UnspentSelector(calculator);
    auto selected = selector.select(utxos, 176360, 1);

    ASSERT_EQ(sum(selected), 0);
    ASSERT_TRUE(selected.size() == 0);
}
