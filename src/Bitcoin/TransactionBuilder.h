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
#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>
#include <iostream>

namespace TW::Bitcoin {

class TransactionBuilder {
public:
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static TransactionPlan plan(const Bitcoin::Proto::SigningInput& input) {
        auto plan = TransactionPlan();
        plan.amount = getTotalAmountFromInput(input);
        auto dest_output_count = 1 + input.extra_outputs_size();
        bool useMaxAmount = input.use_max_amount();
        if (useMaxAmount && dest_output_count > 1) {
            // MaxAmount is not compatible with multiple outputs (not possible to know how to distribute max available amount)
            useMaxAmount = false;
        }
        auto output_size = dest_output_count + 1;   // output(s) + change
        auto calculator =
            UnspentCalculator::getCalculator(static_cast<TWCoinType>(input.coin_type()));
        auto unspentSelector = UnspentSelector(calculator);
        if (useMaxAmount && UnspentSelector::sum(input.utxo()) == plan.amount) {
            assert(dest_output_count == 1);
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
    static Transaction build(const TransactionPlan& plan, 
        const std::vector<std::pair<std::string, int64_t>>& outputs,
        const std::string& changeAddress, enum TWCoinType coin) 
    {
        Transaction tx;
        for (auto i = 0; i < outputs.size(); ++i) {
            auto lockingScriptTo = Script::buildForAddress(std::get<0>(outputs[i]), coin);
            if (lockingScriptTo.empty()) {
                std::cerr << "build locking script: " << coin << ", invalid output address:" << std::get<0>(outputs[i]) << std::endl;
                return {};
            }

            tx.outputs.push_back(TransactionOutput(std::get<1>(outputs[i]), lockingScriptTo));
        }

        if (plan.change > 0) {
            auto lockingScriptChange = Script::buildForAddress(changeAddress, coin);
            tx.outputs.push_back(TransactionOutput(plan.change, lockingScriptChange));
        }

        const auto emptyScript = Script();
        for (auto& utxo : plan.utxos) {
            tx.inputs.emplace_back(utxo.out_point(), emptyScript, utxo.out_point().sequence());
        }

        return tx;
    }

private:
    static int64_t getTotalAmountFromInput(const Bitcoin::Proto::SigningInput& input) {
        int64_t sum = input.amount();
        for (auto i = 0; i < input.extra_outputs_size(); ++i) {
            sum += input.extra_outputs(i).amount();
        }
        return sum;
    }
};

} // namespace TW::Bitcoin
