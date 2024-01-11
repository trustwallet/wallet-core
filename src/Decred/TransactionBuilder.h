// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "../Bitcoin/SigningInput.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../Bitcoin/TransactionBuilder.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Decred.pb.h"

#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>

namespace TW::Decred {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Bitcoin::TransactionPlan plan(const Bitcoin::Proto::SigningInput& input) {
        return Bitcoin::TransactionBuilder::plan(input);
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    static Transaction build(const Bitcoin::TransactionPlan& plan, const Bitcoin::SigningInput& input) {
        auto coin = TWCoinTypeDecred;

        auto lockingScriptTo = Bitcoin::Script::lockScriptForAddress(input.toAddress, coin);
        if (lockingScriptTo.empty()) {
            return {};
        }

        Transaction tx;
        tx.outputs.emplace_back(TransactionOutput(plan.amount, /* version: */ 0, lockingScriptTo));

        if (plan.change > 0) {
            auto lockingScriptChange = Bitcoin::Script::lockScriptForAddress(input.changeAddress, coin);
            tx.outputs.emplace_back(
                TransactionOutput(plan.change, /* version: */ 0, lockingScriptChange));
        }

        const auto emptyScript = Bitcoin::Script();
        for (auto& utxo : plan.utxos) {
            auto txInput = TransactionInput();
            txInput.previousOutput = utxo.outPoint;
            txInput.sequence = utxo.outPoint.sequence;
            tx.inputs.push_back(std::move(txInput));
        }

        // extra outputs
        for (auto& o : input.extraOutputs) {
            auto lockingScriptOther = Bitcoin::Script::lockScriptForAddress(o.first, coin);
            tx.outputs.emplace_back(
                TransactionOutput(o.second, /* version: */ 0, lockingScriptOther));
        }

        return tx;
    }
};

} // namespace TW::Decred
