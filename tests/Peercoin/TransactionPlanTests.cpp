// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Peercoin/OutPoint.h"
#include "Peercoin/Script.h"
#include "Peercoin/UnspentSelector.h"
#include "Peercoin/TransactionPlan.h"
#include "Peercoin/TransactionBuilder.h"
#include "proto/Peercoin.pb.h"
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Peercoin;

auto const txOutPoint = OutPoint(std::vector<uint8_t>(32), 0);

inline auto sum(const std::vector<Proto::UnspentTransaction>& utxos) {
    int64_t s = 0u;
    for (auto& utxo : utxos) {
        s += utxo.amount();
    }
    return s;
}

inline auto buildUTXO(const OutPoint& outPoint, Amount amount) {
    Proto::UnspentTransaction utxo;
    utxo.set_amount(amount);
    utxo.mutable_out_point()->set_hash(outPoint.hash.data(), outPoint.hash.size());
    utxo.mutable_out_point()->set_index(outPoint.index);
    return utxo;
}

inline auto buildSigningInput(Amount amount, int byteFee, const std::vector<Proto::UnspentTransaction>& utxos, bool useMaxAmount, enum TWCoinType coin) {
    Proto::SigningInput input;
    input.set_amount(amount);
    input.set_byte_fee(byteFee);
    input.set_use_max_amount(useMaxAmount);
    input.set_coin_type(coin);
    *input.mutable_utxo() = { utxos.begin(), utxos.end() };
    return input;
}

TEST(PeercoinTransactionPlan, NonMaxAmount) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(txOutPoint, 4000));
    utxos.push_back(buildUTXO(txOutPoint, 2000));
    utxos.push_back(buildUTXO(txOutPoint, 6000));
    utxos.push_back(buildUTXO(txOutPoint, 1000));
    utxos.push_back(buildUTXO(txOutPoint, 50000));
    utxos.push_back(buildUTXO(txOutPoint, 120000));

    auto sigingInput = buildSigningInput(10000, 1, utxos, false, TWCoinTypePeercoin);
    auto txPlan = TransactionBuilder::plan(sigingInput);

    ASSERT_EQ(txPlan.amount, 10000);
    ASSERT_EQ(txPlan.change, 30000);
    //ASSERT_EQ(txPlan.change, 39774);
}

TEST(PeercoinTransactionPlan, MaxAmount) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(txOutPoint, 4000));
    utxos.push_back(buildUTXO(txOutPoint, 2000));
    utxos.push_back(buildUTXO(txOutPoint, 15000));
    utxos.push_back(buildUTXO(txOutPoint, 15000));
    utxos.push_back(buildUTXO(txOutPoint, 3000));
    utxos.push_back(buildUTXO(txOutPoint, 200));

    ASSERT_EQ(sum(utxos), 39200);

    auto sigingInput = buildSigningInput(39200, 32, utxos, true, TWCoinTypePeercoin);
    auto txPlan = TransactionBuilder::plan(sigingInput);

    //ASSERT_EQ(txPlan.availableAmount, 30000);
    ASSERT_EQ(txPlan.availableAmount, 39200);
    //ASSERT_EQ(txPlan.amount, 19120);
    ASSERT_EQ(txPlan.amount, 29200);
    ASSERT_EQ(txPlan.change, 0);
    //ASSERT_EQ(txPlan.fee, 10880);
    ASSERT_EQ(txPlan.fee, 10000);
}
