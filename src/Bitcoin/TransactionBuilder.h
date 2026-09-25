// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "SigningInput.h"
#include "Transaction.h"
#include "TransactionPlan.h"
#include "InputSelector.h"
#include "../Result.h"
#include "../proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWCoinType.h>

#include <optional>
#include <algorithm>

namespace TW::Bitcoin {

class TransactionBuilder {
public:
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static TransactionPlan plan(const SigningInput& input);

    /// Builds a transaction with the selected input UTXOs, and one main output and an optional change output.
    template <typename Transaction>
    static Result<Transaction, Common::Proto::SigningError> build(const TransactionPlan& plan, const SigningInput& input) {
        Transaction tx;
        tx.lockTime = input.lockTime;

        auto outputTo = prepareOutputWithScript(input.toAddress, plan.amount, input.coinType);
        if (!outputTo.has_value()) { 
            return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_address);
        }
        tx.outputs.push_back(outputTo.value());

        if (plan.change > 0) {
            auto outputChange = prepareOutputWithScript(input.changeAddress, plan.change, input.coinType);
            if (!outputChange.has_value()) { 
                return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_address);
            }
            tx.outputs.push_back(outputChange.value());
        }

        const auto emptyScript = Script();
        for (auto& utxo : plan.utxos) {
            tx.inputs.emplace_back(utxo.outPoint, emptyScript, utxo.outPoint.sequence);
        }

        // Optional OP_RETURN output
        if (!plan.outputOpReturn.empty()) {
            auto lockingScriptOpReturn = Script::buildOpReturnScript(plan.outputOpReturn);
            if (lockingScriptOpReturn.bytes.empty()) {
                return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_memo);
            }

            auto emplace_at = tx.outputs.end();
            if (plan.outputOpReturnIndex.has_value()) {
                emplace_at = tx.outputs.begin();
                std::advance(emplace_at, plan.outputOpReturnIndex.value());
            }
            int64_t amount = 0;
            tx.outputs.emplace(emplace_at, amount, lockingScriptOpReturn);
        }

        // extra outputs (always in the end of the outputs list)
        for (auto& o : input.extraOutputs) {
            auto output = prepareOutputWithScript(o.first, o.second, input.coinType);
            if (!output.has_value()) { 
                return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_address);
            }
            tx.outputs.push_back(output.value());
        }

        return Result<Transaction, Common::Proto::SigningError>(tx);
    }

    /// Prepares a TransactionOutput with given address and amount, prepares script for it
    static std::optional<TransactionOutput> prepareOutputWithScript(std::string address, Amount amount, enum TWCoinType coin);

    /// The maximum number of UTXOs to consider.  UTXOs above this limit are cut off because it cak take very long.
    static const size_t MaxUtxosHardLimit;
};

} // namespace TW::Bitcoin
