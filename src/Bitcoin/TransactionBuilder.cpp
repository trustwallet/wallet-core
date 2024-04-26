// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionBuilder.h"
#include "Script.h"
#include "TransactionSigner.h"
#include "SignatureBuilder.h"

#include "../Coin.h"

#include <algorithm>
#include <cassert>

namespace TW::Bitcoin {


// Above this number of UTXOs a simplified selection is used (optimization)
static const auto SimpleModeLimit = 1000;
// The maximum number of UTXOs to consider.  UTXOs above this limit are cut off because it cak take very long
const size_t TransactionBuilder::MaxUtxosHardLimit = 3000;

std::optional<TransactionOutput> TransactionBuilder::prepareOutputWithScript(std::string address, Amount amount, enum TWCoinType coin) {
    auto lockingScript = Script::lockScriptForAddress(address, coin);
    if (lockingScript.empty()) {
        return {};
    }
    return TransactionOutput(amount, lockingScript);
}


/// Estimate encoded size by simple formula
int64_t estimateSimpleFee(const FeeCalculator& feeCalculator, const TransactionPlan& plan, int outputSize, int64_t byteFee) {
    return feeCalculator.calculate(plan.utxos.size(), outputSize, byteFee);
}

/// Estimate encoded size by invoking sign(sizeOnly), get actual size
int64_t estimateSegwitFee(const FeeCalculator& feeCalculator, const TransactionPlan& plan, int outputSize, const SigningInput& input) {
    TWPurpose coinPurpose = TW::purpose(static_cast<TWCoinType>(input.coinType));
    if (coinPurpose != TWPurposeBIP84) {
        // not segwit, return default simple estimate
        return estimateSimpleFee(feeCalculator, plan, outputSize, input.byteFee);
    }

    // duplicate input, with the current plan
    auto inputWithPlan = std::move(input);
    inputWithPlan.plan = plan;

    auto result = TransactionSigner<Transaction, TransactionBuilder>::sign(inputWithPlan, SigningMode_SizeEstimationOnly);
    if (!result) {
        // signing failed; return default simple estimate
        return estimateSimpleFee(feeCalculator, plan, outputSize, input.byteFee);
    }

    // Obtain the encoded size
    auto transaction = result.payload();
    Data dataNonSegwit;
    transaction.encode(dataNonSegwit, Transaction::SegwitFormatMode::NonSegwit);
    int64_t sizeNonSegwit = dataNonSegwit.size();
    uint64_t vSize = 0;
    // Check if there is segwit
    if (!transaction.hasWitness()) {
        // no segwit, virtual size is defined as non-segwit size
        vSize = sizeNonSegwit;
    } else {
        Data dataWitness;
        transaction.encodeWitness(dataWitness);
        int64_t witnessSize = 2 + dataWitness.size();
        // compute virtual size:  (smaller) non-segwit + 1/4 of the diff (witness-only)
        // (in other way: 3/4 of (smaller) non-segwit + 1/4 of segwit size)
        vSize = sizeNonSegwit + witnessSize/4 + (witnessSize % 4 != 0);
    }
    uint64_t fee = input.byteFee * vSize;

    return fee;
}

int extraOutputCount(const SigningInput& input) {
    int count = int(input.outputOpReturn.size() > 0);
    return count + int(input.extraOutputs.size());
}

TransactionPlan TransactionBuilder::plan(const SigningInput& input) {
    TransactionPlan plan;
    if (input.outputOpReturn.size() > 0) {
        plan.outputOpReturn = input.outputOpReturn;
    }
    plan.outputOpReturnIndex = input.outputOpReturnIndex;

    bool maxAmount = input.useMaxAmount;
    Amount totalAmount = input.amount + input.extraOutputsAmount;
    Amount dustThreshold = input.dustCalculator->dustAmount(input.byteFee);

    if (totalAmount == 0 && !maxAmount) {
        plan.error = Common::Proto::Error_zero_amount_requested;
    } else if (input.utxos.empty()) {
        plan.error = Common::Proto::Error_missing_input_utxos;
    } else {
        const auto& feeCalculator = getFeeCalculator(static_cast<TWCoinType>(input.coinType), input.disableDustFilter);
        auto inputSelector = InputSelector<UTXO>(input.utxos, feeCalculator, input.dustCalculator);
        auto inputSum = InputSelector<UTXO>::sum(input.utxos);

        // select UTXOs
        plan.amount = input.amount;

        // if amount requested is the same or more than available amount, it cannot be satisfied, but
        // treat this case as MaxAmount, and send maximum available (which will be less)
        if (!maxAmount && static_cast<uint64_t>(totalAmount) >= inputSum) {
            maxAmount = true;
        }

        auto extraOutputs = extraOutputCount(input);
        auto output_size = 2;
        UTXOs selectedInputs;
        if (!maxAmount) {
            // Please note that there may not be a "change" output if the "change.amount" is less than "dust",
            // but we use a max amount of transaction outputs to simplify the algorithm, so the fee can be slightly bigger in rare cases.
            output_size = 2 + extraOutputs; // output + change
            if (input.useMaxUtxo) {
                selectedInputs = inputSelector.selectMaxAmount(input.byteFee);
            } else if (input.utxos.size() <= SimpleModeLimit &&
                input.utxos.size() <= MaxUtxosHardLimit) {
                selectedInputs = inputSelector.select(totalAmount, input.byteFee, output_size);
            } else {
                selectedInputs =
                    inputSelector.selectSimple(totalAmount, input.byteFee, output_size);
            }
        } else {
            output_size = 1 + extraOutputs; // output, no change
            selectedInputs = inputSelector.selectMaxAmount(input.byteFee);
        }
        if (selectedInputs.size() <= MaxUtxosHardLimit) {
            plan.utxos = selectedInputs;
        } else {
            // truncate to limit number of selected UTXOs
            plan.utxos.clear();
            for (auto i = 0ul; i < MaxUtxosHardLimit; ++i) {
                plan.utxos.push_back(selectedInputs[i]);
            }
        }

        if (plan.utxos.empty()) {
            plan.amount = 0;
            plan.error = Common::Proto::Error_not_enough_utxos;
        } else if (maxAmount && !input.extraOutputs.empty()) {
            // As of now, we don't support `max` amount **and** extra outputs.
            plan.amount = 0;
            plan.error = Common::Proto::Error_invalid_params;
        } else {
            plan.availableAmount = InputSelector<UTXO>::sum(plan.utxos);

            // There can be less UTXOs after Dust filtering.
            if (!maxAmount && totalAmount > plan.availableAmount) {
                TransactionPlan errorPlan;
                errorPlan.error = Common::Proto::Error_not_enough_utxos;
                return errorPlan;
            }

            // Compute fee.
            // must preliminary set change so that there is a second output
            if (!maxAmount) {
                plan.amount = input.amount;
                plan.fee = 0;
                plan.change = plan.availableAmount - totalAmount;
            } else {
                plan.amount = plan.availableAmount;
                plan.fee = 0;
                plan.change = 0;
            }
            plan.fee = estimateSegwitFee(feeCalculator, plan, output_size, input);

            // `InputSelector` has a rough segwit fee estimation algorithm,
            // so the fee could be increased or decreased (see `InputSelector::select`).
            // We need to make sure if we have enough UTXOs to cover "requested amount + final fee".
            if (!maxAmount && plan.availableAmount < plan.fee + plan.amount) {
                TransactionPlan errorPlan;
                errorPlan.error = Common::Proto::Error_not_enough_utxos;
                return errorPlan;
            }

            // If fee is larger than availableAmount (can happen in special maxAmount case), we reduce it (and hope it will go through)
            plan.fee = std::min(plan.availableAmount, plan.fee);
            assert(plan.fee >= 0 && plan.fee <= plan.availableAmount);

            // adjust/compute amount
            if (!maxAmount) {
                // reduce amount if needed
                plan.amount = std::max(Amount(0), std::min(plan.amount, plan.availableAmount - plan.fee));
            } else {
                // max available amount
                plan.amount = std::max(Amount(0), plan.availableAmount - plan.fee);
            }
            assert(plan.amount >= 0 && plan.amount <= plan.availableAmount);

            // The total amount that will be spent.
            Amount totalSpendAmount = plan.amount + input.extraOutputsAmount + plan.fee;

            // Make sure that the output amount is greater or at least equal to the dust threshold.
            if (plan.amount < dustThreshold) {
                TransactionPlan errorPlan;
                errorPlan.error = maxAmount ? Common::Proto::Error_not_enough_utxos : Common::Proto::Error_dust_amount_requested;
                return errorPlan;
            }

            // Make sure that we have enough available UTXOs to spend `fee`, `amount` and `extraOutputsAmount`.
            if (plan.availableAmount < totalSpendAmount) {
                TransactionPlan errorPlan;
                errorPlan.error = Common::Proto::Error_not_enough_utxos;
                return errorPlan;
            }

            auto changeAmount = plan.availableAmount - totalSpendAmount;
            // Compute change if it's not dust.
            if (changeAmount >= dustThreshold) {
                plan.change = changeAmount;
            } else {
                // Spend the change as tx fee if it's dust, otherwise the transaction won't be mined.
                plan.change = 0;
                plan.fee += changeAmount;
            }
        }
    }
    assert(plan.change >= 0 && plan.change <= plan.availableAmount);
    assert(!maxAmount || plan.change == 0); // change is 0 in max amount case

    assert(plan.error != Common::Proto::OK
           // `plan.error` is OK, check if the values are expected.
           || plan.amount + input.extraOutputsAmount + plan.change + plan.fee == plan.availableAmount);

    return plan;
}

} // namespace TW::Bitcoin
