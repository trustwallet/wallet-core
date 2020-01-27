// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include "Peercoin/OutPoint.h"
#include "Peercoin/Script.h"
#include "Peercoin/UnspentSelector.h"
#include "proto/Peercoin.pb.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Peercoin;

auto transactionPeercoinOutPoint = OutPoint(std::vector<uint8_t>(32), 0);

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

TEST(PeercoinUnspentSelector, SelectUnpsents1) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 6000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 1000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 11000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 12000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 5000, 1);

    ASSERT_EQ(sum(selected), 11000);
}

TEST(PeercoinUnspentSelector, SelectUnpsents2) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 6000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 1000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 50000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 120000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10000, 1);

    ASSERT_EQ(sum(selected), 50000);
}

TEST(PeercoinUnspentSelector, SelectUnpsents3) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 5000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 6000, 1);

    ASSERT_EQ(sum(selected), 9000);
}

TEST(PeercoinUnspentSelector, SelectUnpsents4) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 40000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 30000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 30000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 50000, 1);

    ASSERT_EQ(sum(selected), 70000);
}

TEST(PeercoinUnspentSelector, SelectUnpsents5) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 1000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 2000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 3000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 5000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 6000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 7000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 8000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 9000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 28000, 1);

    ASSERT_EQ(sum(selected), 30000);
}

TEST(PeercoinUnspentSelector, SelectUnpsentsInsufficient) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 4000));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 15000, 1);

    ASSERT_TRUE(selected.empty());
}

TEST(PeercoinUnspentSelector, SelectCustomCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 794121));
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 2289357));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 2287189, 61);

    ASSERT_EQ(sum(selected), 3083478);
}

TEST(PeercoinUnspentSelector, SelectMaxCase) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(transactionPeercoinOutPoint, 10189534));

    auto selector = UnspentSelector();
    auto selected = selector.select(utxos, 10189342, 1);

    ASSERT_EQ(sum(selected), 10189534);
    ASSERT_TRUE(selected.size() > 0);
}
