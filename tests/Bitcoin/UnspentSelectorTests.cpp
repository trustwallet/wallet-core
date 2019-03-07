// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

inline auto buildUTXO(const OutPoint& outPoint, int64_t amount) {
    Proto::UnspentTransaction utxo;
    utxo.set_amount(amount);
    utxo.mutable_out_point()->set_hash(outPoint.hash.data(), outPoint.hash.size());
    utxo.mutable_out_point()->set_index(outPoint.index);
    return utxo;
}

TEST(UnspentSelector, SelectUnpsents1) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionOutPoint, 6000));
    utxos.push_back(buildUTXO(transactionOutPoint, 1000));
    utxos.push_back(buildUTXO(transactionOutPoint, 11000));
    utxos.push_back(buildUTXO(transactionOutPoint, 12000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 5000, 1);

    ASSERT_EQ(sum(selected), 11000);
}

TEST(UnspentSelector, SelectUnpsents2) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionOutPoint, 6000));
    utxos.push_back(buildUTXO(transactionOutPoint, 1000));
    utxos.push_back(buildUTXO(transactionOutPoint, 50000));
    utxos.push_back(buildUTXO(transactionOutPoint, 120000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10000, 1);

    ASSERT_EQ(sum(selected), 50000);
}

TEST(UnspentSelector, SelectUnpsents3) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionOutPoint, 5000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 6000, 1);

    ASSERT_EQ(sum(selected), 9000);
}

TEST(UnspentSelector, SelectUnpsents4) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 40000));
    utxos.push_back(buildUTXO(transactionOutPoint, 30000));
    utxos.push_back(buildUTXO(transactionOutPoint, 30000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50000, 1);

    ASSERT_EQ(sum(selected), 70000);
}

TEST(UnspentSelector, SelectUnpsents5) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 1000));
    utxos.push_back(buildUTXO(transactionOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionOutPoint, 3000));
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 5000));
    utxos.push_back(buildUTXO(transactionOutPoint, 6000));
    utxos.push_back(buildUTXO(transactionOutPoint, 7000));
    utxos.push_back(buildUTXO(transactionOutPoint, 8000));
    utxos.push_back(buildUTXO(transactionOutPoint, 9000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 28000, 1);

    ASSERT_EQ(sum(selected), 30000);
}

TEST(UnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15000, 1);

    ASSERT_TRUE(selected.empty());
}

TEST(UnspentSelector, SelectCustomCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 794121));
    utxos.push_back(buildUTXO(transactionOutPoint, 2289357));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 2287189, 61);

    ASSERT_EQ(sum(selected), 3083478);
}

TEST(UnspentSelector, SelectMaxCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 10189534));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10189342, 1);

    ASSERT_EQ(sum(selected), 10189534);
    ASSERT_TRUE(selected.size() > 0);
}
