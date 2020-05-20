// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionBuilder.h"
#include "TransactionSigner.h"

#include <algorithm>
#include <cassert>

namespace TW::Bitcoin {

TransactionPlan TransactionBuilder::plan(const Bitcoin::Proto::SigningInput& input) {
        auto plan = TransactionPlan();
        plan.amount = input.amount();

        auto output_size = 2; // output + change
        auto& feeCalculator = getFeeCalculator(static_cast<TWCoinType>(input.coin_type()));
        auto unspentSelector = UnspentSelector(feeCalculator);
        if (input.use_max_amount() && UnspentSelector::sum(input.utxo()) == plan.amount) {
        //if (input.use_max_amount()) {
            output_size = 1; // no change
            Amount newAmount = 0;
            auto input_size = 0;

            // take sum of all UTXOs, except dust ones, as target amount
            for (auto utxo : input.utxo()) {
                if (utxo.amount() >
                    feeCalculator.calculateSingleInput(input.byte_fee())) {
                    input_size++;
                    newAmount += utxo.amount();
                }
            }
            std::cerr << "TransactionBuilder::plan " << newAmount << " " << UnspentSelector::sum(input.utxo()) << "\n";

            plan.amount = newAmount - feeCalculator.calculate(input_size, output_size, input.byte_fee());
            plan.amount = std::max(Amount(0), plan.amount);
        }

        plan.utxos = unspentSelector.select(input.utxo(), plan.amount, input.byte_fee(), output_size);
        plan.availableAmount = UnspentSelector::sum(plan.utxos);

        plan.fee = std::min(plan.availableAmount,
            feeCalculator.calculate(plan.utxos.size(), output_size, input.byte_fee()));
        assert(plan.fee >= 0 && plan.fee <= plan.availableAmount);

        if (input.use_max_amount()) {
            // max_amount case
            plan.amount = std::max(Amount(0), plan.availableAmount - plan.fee);
            assert(plan.amount >= 0 && plan.amount <= plan.availableAmount);
            plan.change = 0;
        } else {
            // reduce amount if needed
            plan.amount = std::max(Amount(0), std::min(plan.amount, plan.availableAmount - plan.fee));
            assert(plan.amount >= 0 && plan.amount <= plan.availableAmount);
            plan.change = plan.availableAmount - plan.amount - plan.fee;
        }
        assert(plan.amount + plan.change + plan.fee == plan.availableAmount);

        return plan;
}

} // namespace TW::Bitcoin
