// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "gtest/gtest.h"
#include "../src/Bitcoin/OutPoint.h"
#include "../src/Bitcoin/Script.h"
#include "../src/Bitcoin/UnspentSelector.h"
#include "../src/proto/Bitcoin.pb.h"

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
    utxo.mutable_out_point()->set_hash(outPoint.hash, 32);
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

    auto selected = UnspentSelector::select(utxos, 5000, 1);
    
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

    auto selected = UnspentSelector::select(utxos, 10000, 1);

    ASSERT_EQ(sum(selected), 50000);
}

TEST(UnspentSelector, SelectUnpsents3) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionOutPoint, 5000));

    auto selected = UnspentSelector::select(utxos, 6000, 1);

    ASSERT_EQ(sum(selected), 9000);
}

TEST(UnspentSelector, SelectUnpsents4) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 40000));
    utxos.push_back(buildUTXO(transactionOutPoint, 30000));
    utxos.push_back(buildUTXO(transactionOutPoint, 30000));

    auto selected = UnspentSelector::select(utxos, 50000, 1);

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

    auto selected = UnspentSelector::select(utxos, 28000, 1);

    ASSERT_EQ(sum(selected), 30000);
}

TEST(UnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionOutPoint, 4000));

    auto selected = UnspentSelector::select(utxos, 15000, 1);

    ASSERT_TRUE(selected.empty());
}
