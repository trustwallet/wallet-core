// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../Bitcoin/UnspentSelector.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Decred.pb.h"

#include <algorithm>

namespace TW::Decred {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Bitcoin::TransactionPlan plan(const Bitcoin::Proto::SigningInput& input) {
        auto plan = Bitcoin::TransactionPlan();
        plan.amount = input.amount();

        auto output_size = 2;
        auto calculator =
            Bitcoin::UnspentCalculator::getCalculator(static_cast<TWCoinType>(input.coin_type()));
        auto unspentSelector = Bitcoin::UnspentSelector(calculator);
        if (input.use_max_amount() && Bitcoin::UnspentSelector::sum(input.utxo()) == plan.amount) {
            output_size = 1;
            int64_t newAmount = 0;
            auto input_size = 0;

            for (auto utxo : input.utxo()) {
                if (utxo.amount() >
                    unspentSelector.calculator.calculateSingleInput(input.byte_fee())) {
                    input_size++;
                    newAmount += utxo.amount();
                }
            }

            plan.amount = newAmount - unspentSelector.calculator.calculate(input_size, output_size,
                                                                           input.byte_fee());
            plan.amount = std::max(Bitcoin::Amount(0), plan.amount);
        }

        plan.utxos =
            unspentSelector.select(input.utxo(), plan.amount, input.byte_fee(), output_size);
        plan.fee =
            unspentSelector.calculator.calculate(plan.utxos.size(), output_size, input.byte_fee());

        plan.availableAmount = Bitcoin::UnspentSelector::sum(plan.utxos);

        if (plan.amount > plan.availableAmount - plan.fee) {
            plan.amount = std::max(Bitcoin::Amount(0), plan.availableAmount - plan.fee);
        }

        plan.change = plan.availableAmount - plan.amount - plan.fee;

        return plan;
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    static Transaction build(const Bitcoin::TransactionPlan& plan, const std::string& toAddress,
                             const std::string& changeAddress) {
        auto lockingScriptTo = Bitcoin::Script::buildForAddress(toAddress);
        if (lockingScriptTo.empty()) {
            return {};
        }

        Transaction tx;
        tx.outputs.emplace_back(TransactionOutput(plan.amount, /* version: */ 0, lockingScriptTo));

        if (plan.change > 0) {
            auto lockingScriptChange = Bitcoin::Script::buildForAddress(changeAddress);
            tx.outputs.emplace_back(
                TransactionOutput(plan.change, /* version: */ 0, lockingScriptChange));
        }

        const auto emptyScript = Bitcoin::Script();
        for (auto& utxo : plan.utxos) {
            auto input = TransactionInput();
            input.previousOutput = utxo.out_point();
            input.sequence = utxo.out_point().sequence();
            input.sequence = utxo.out_point().sequence();
            tx.inputs.push_back(std::move(input));
        }

        return tx;
    }
};

} // namespace TW::Decred
