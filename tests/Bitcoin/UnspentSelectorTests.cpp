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

TEST(BitcoinUnspentSelector, SelectUnpsents1) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 2000, 6000, 1000, 11000, 12000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 5000, 1);

    ASSERT_TRUE(verifySelected(selected, {11000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents2) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 2000, 6000, 1000, 50000, 120000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10000, 1);

    ASSERT_TRUE(verifySelected(selected, {50000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents3) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 2000, 5000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 6000, 1);

    ASSERT_TRUE(verifySelected(selected, {4000, 5000}));
}

TEST(BitcoinUnspentSelector, SelectUnpsents4) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {40000, 30000, 30000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50000, 1);

    ASSERT_EQ(sum(selected), 70000);
}

TEST(BitcoinUnspentSelector, SelectUnpsents5) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 28000, 1);

    ASSERT_EQ(sum(selected), 30000);
}

TEST(BitcoinUnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {4000, 4000, 4000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15000, 1);

    ASSERT_TRUE(selected.empty());
}

TEST(BitcoinUnspentSelector, SelectCustomCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {794121, 2289357});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 2287189, 61);

    ASSERT_EQ(sum(selected), 3083478);
}

TEST(BitcoinUnspentSelector, SelectNegativeNoUtxo) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100000, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectNegativeTarget0) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 0, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneTypical) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50'000, 1);

    ASSERT_TRUE(verifySelected(selected, {100'000}));
}

TEST(BitcoinUnspentSelector, SelectOneInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 200'000, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneInsufficientEqual) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 100'000, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneInsufficientHigher) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 99'900, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneFitsExactly) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 100'000 - 192, 1); // shuold be 226

    ASSERT_TRUE(verifySelected(selected, {100'000}));

    ASSERT_EQ(feeCalculator.calculate(1, 2, 1), 226);
    ASSERT_EQ(feeCalculator.calculate(1, 1, 1), 192);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - 192 + 1, 1);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectOneFitsExactlyHighfee) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100'000});

    const auto byteFee = 10;
    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 100'000 - 1920, byteFee); // shuold be 2260

    ASSERT_TRUE(verifySelected(selected, {100'000}));

    ASSERT_EQ(feeCalculator.calculate(1, 2, byteFee), 2260);
    ASSERT_EQ(feeCalculator.calculate(1, 1, byteFee), 1920);

    // 1 sat more and does not fit any more
    selected = selector.select(utxos, 100'000 - 1920 + 1, byteFee);

    ASSERT_TRUE(verifySelected(selected, {}));
}

TEST(BitcoinUnspentSelector, SelectThreeNoDust) {
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

TEST(BitcoinUnspentSelector, SelectTwoFirstEnough) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15'000, 1);

    ASSERT_TRUE(verifySelected(selected, {20'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoSecondEnough) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 70'000, 1);

    ASSERT_TRUE(verifySelected(selected, {80'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoBoth) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 80'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 90'000, 1);

    ASSERT_TRUE(verifySelected(selected, {20'000, 80'000}));
}

TEST(BitcoinUnspentSelector, SelectTwoFirstEnoughButSecond) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {20'000, 22'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 18'000, 1);

    ASSERT_TRUE(verifySelected(selected, {22'000}));
}

TEST(BitcoinUnspentSelector, SelectTenThree) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 300'000, 1);

    ASSERT_TRUE(verifySelected(selected, {100'000, 125'000, 150'000}));
}

TEST(BitcoinUnspentSelector, SelectTenThreeExact) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {1'000, 2'000, 100'000, 3'000, 4'000, 5,000, 125'000, 6'000, 150'000, 7'000});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 375'000 - 522 - 546, 1);

    ASSERT_TRUE(verifySelected(selected, {100'000, 125'000, 150'000}));

    ASSERT_EQ(feeCalculator.calculate(3, 2, 1), 522);

    // one more, and it's too much
    selected = selector.select(utxos, 375'000 - 522 - 546 + 1, 1);

    ASSERT_TRUE(verifySelected(selected, {7'000, 100'000, 125'000, 150'000}));
}

TEST(BitcoinUnspentSelector, SelectMaxCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {10189534});

    auto& feeCalculator = getFeeCalculator(TWCoinTypeBitcoin);
    auto selector = UnspentSelector(feeCalculator);
    auto selected = selector.select(utxos, 10189534 - 226, 1);

    ASSERT_TRUE(verifySelected(selected, {10189534}));

    ASSERT_EQ(feeCalculator.calculate(1, 2, 1), 226);
}

TEST(BitcoinUnspentSelector, SelectZcashUnpsents) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 10000, 1);

    ASSERT_EQ(sum(selected), 73774);
    ASSERT_TRUE(selected.size() > 0);
}

TEST(BitcoinUnspentSelector, SelectGroestlUnpsents) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {499971976});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 499951976, 1, 1);

    ASSERT_EQ(sum(selected), 499971976);
    ASSERT_TRUE(selected.size() > 0);
}

TEST(BitcoinUnspentSelector, SelectZcashMaxUnpsents) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 166366, 1);

    ASSERT_EQ(sum(selected), 176366);
    ASSERT_TRUE(selected.size() > 0);
}

TEST(BitcoinUnspentSelector, SelectZcashMaxUnpsents2) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    buildUTXOs(utxos, {100000, 2592, 73774});

    auto selector = UnspentSelector(getFeeCalculator(TWCoinTypeZcash));
    auto selected = selector.select(utxos, 176360, 1);

    ASSERT_EQ(sum(selected), 0);
    ASSERT_TRUE(selected.size() == 0);
}
