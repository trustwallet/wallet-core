// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TxComparisonHelper.h"

#include <TrustWalletCore/TWCoinType.h>

#include "Bitcoin/OutPoint.h"
#include "proto/Bitcoin.pb.h"
#include "Data.h"

#include <gtest/gtest.h>
#include <sstream>

using namespace TW;
using namespace TW::Bitcoin;

auto emptyTxOutPoint = OutPoint(Data(32), 0);

Proto::UnspentTransaction buildTestUTXO(int64_t amount) {
    Proto::UnspentTransaction utxo;
    utxo.set_amount(amount);
    const auto& outPoint = emptyTxOutPoint;
    utxo.mutable_out_point()->set_hash(outPoint.hash.data(), outPoint.hash.size());
    utxo.mutable_out_point()->set_index(outPoint.index);
    return utxo;
}

std::vector<Proto::UnspentTransaction> buildTestUTXOs(const std::vector<int64_t>& amounts) {
    std::vector<Proto::UnspentTransaction> utxos;
    for (auto it = amounts.begin(); it != amounts.end(); it++) {
        utxos.push_back(buildTestUTXO(*it));
    }
    return utxos;
}

Proto::SigningInput buildSigningInput(Amount amount, int byteFee, const std::vector<Proto::UnspentTransaction>& utxos, bool useMaxAmount, enum TWCoinType coin) {
    Proto::SigningInput input;
    input.set_amount(amount);
    input.set_byte_fee(byteFee);
    input.set_use_max_amount(useMaxAmount);
    input.set_coin_type(coin);
    *input.mutable_utxo() = { utxos.begin(), utxos.end() };
    return input;
}

int64_t sumUTXOs(const std::vector<Proto::UnspentTransaction>& utxos) {
    int64_t s = 0u;
    for (auto& utxo: utxos) {
        s += utxo.amount();
    }
    return s;
}

std::string verifySelectedUTXOs(const std::vector<Proto::UnspentTransaction>& selected, const std::vector<int64_t>& expectedAmounts) {
    std::stringstream ss;
    if (selected.size() != expectedAmounts.size()) {
        ss << "Wrong number of selected UTXOs, " << selected.size() << " vs. " << expectedAmounts.size() << std::endl;
    }
    for (auto i = 0; i < selected.size() && i < expectedAmounts.size(); ++i) {
        if (expectedAmounts[i] != selected[i].amount()) {
            ss << "Wrong UTXOs amount, pos " << i << " amount " << selected[i].amount() << " expected " << expectedAmounts[i] << std::endl;
        }
    }
    return ss.str();
}

std::string verifyPlan(const TransactionPlan& plan, const std::vector<int64_t>& utxoAmounts, int64_t outputAmount, int64_t fee) {
    std::stringstream ss;
    ss << verifySelectedUTXOs(plan.utxos, utxoAmounts);
    if (plan.amount != outputAmount) {
        ss << "Mismatch in amount, act " << plan.amount << ", exp " << outputAmount << "\n";
    }
    if (plan.fee != fee) {
        ss << "Mismatch in fee, act " << plan.fee << ", exp " << fee << "\n";
    }
    int64_t sumExpectedUTXOs = 0;
    for (auto i = 0; i < utxoAmounts.size(); ++i) {
        sumExpectedUTXOs += utxoAmounts[i];
    }
    if (plan.availableAmount != sumExpectedUTXOs) {
        ss << "Mismatch in availableAmount, act " << plan.availableAmount << ", exp " << sumExpectedUTXOs << "\n";
    }
    int64_t expectedChange = sumExpectedUTXOs - outputAmount - fee;
    if (plan.change != expectedChange) {
        ss << "Mismatch in change, act " << plan.change << ", exp " << expectedChange << "\n";
    }
    return ss.str();
}
