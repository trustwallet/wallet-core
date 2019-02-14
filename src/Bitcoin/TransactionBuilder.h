// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "TransactionPlan.h"
#include "UnspentSelector.h"
#include "../proto/Bitcoin.pb.h"

#include <algorithm>

namespace TW {
namespace Bitcoin {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static TransactionPlan plan(Bitcoin::Proto::SigningInput input) {
        auto plan = TransactionPlan();
        plan.amount = input.amount();

        if (input.use_max_amount() && UnspentSelector::sum(input.utxo()) == plan.amount) {
            plan.amount -= UnspentSelector::calculateFee(input.utxo().size(), 2, input.byte_fee());
            plan.amount = std::max(Amount(0), plan.amount);
        }

        plan.utxos = UnspentSelector::select(input.utxo(), plan.amount, input.byte_fee());
        plan.fee = UnspentSelector::calculateFee(plan.utxos.size(), 2, input.byte_fee());        

        plan.availableAmount = UnspentSelector::sum(plan.utxos);

        if (plan.amount >  plan.availableAmount - plan.fee) {
            plan.amount = std::max(Amount(0),  plan.availableAmount - plan.fee);
        }

        plan.change =  plan.availableAmount - plan.amount - plan.fee;

        return plan;
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    template<typename Transaction>
    static Transaction build(const TransactionPlan& plan, const std::string& toAddress, const std::string& changeAddress) {
        auto lockingScriptTo = Script::buildForAddress(toAddress);
        if (lockingScriptTo.empty()) {
            return {};
        }

        Transaction tx;
        tx.outputs.push_back(TransactionOutput(plan.amount, lockingScriptTo));

        if (plan.change > 0) {
            auto lockingScriptChange = Script::buildForAddress(changeAddress);
            tx.outputs.push_back(TransactionOutput(plan.change, lockingScriptChange));
        }

        const auto emptyScript = Script();
        for (auto& utxo : plan.utxos) {
            tx.inputs.emplace_back(utxo.out_point(), emptyScript, utxo.out_point().sequence());
        }

        return tx;
    }
};

}} //namespace
