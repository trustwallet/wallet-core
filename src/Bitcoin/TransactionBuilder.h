// Copyright © 2017-2019 Trust Wallet.
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

namespace TW::Bitcoin {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static TransactionPlan plan(const Bitcoin::Proto::SigningInput& input) {
        auto plan = TransactionPlan();
        plan.amount = input.amount();

        auto output_size = 2;
        auto calculator =
            UnspentCalculator::getCalculator(static_cast<TWCoinType>(input.coin_type()));
        auto unspentSelector = UnspentSelector(calculator);
        if (input.use_max_amount() && UnspentSelector::sum(input.utxo()) == plan.amount) {
            output_size = 1;
            Amount newAmount = 0;
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
            plan.amount = std::max(Amount(0), plan.amount);
        }

        plan.utxos =
            unspentSelector.select(input.utxo(), plan.amount, input.byte_fee(), output_size);
        plan.fee =
            unspentSelector.calculator.calculate(plan.utxos.size(), output_size, input.byte_fee());

        plan.availableAmount = UnspentSelector::sum(plan.utxos);

        if (plan.amount > plan.availableAmount - plan.fee) {
            plan.amount = std::max(Amount(0), plan.availableAmount - plan.fee);
        }

        plan.change = plan.availableAmount - plan.amount - plan.fee;

        return plan;
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    template <typename Transaction>
    static Transaction build(const TransactionPlan& plan, const std::string& toAddress,
                             const std::string& changeAddress) {
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

} // namespace TW::Bitcoin
