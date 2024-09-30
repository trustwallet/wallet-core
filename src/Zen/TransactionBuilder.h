// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "../Bitcoin/Transaction.h"
#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../Bitcoin/TransactionOutput.h"
#include "../Coin.h"
#include "../proto/Bitcoin.pb.h"
#include  "../HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>

using namespace TW;

namespace TW::Zen {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Bitcoin::TransactionPlan plan(const Bitcoin::SigningInput& input) {
        return Bitcoin::TransactionBuilder::plan(input);
    }

    /// Builds a transaction with the selected input UTXOs, and one main output and an optional change output.
    template <typename Transaction>
    static Result<Transaction, Common::Proto::SigningError> build(const Bitcoin::TransactionPlan& plan, const Bitcoin::SigningInput& input) {
        Transaction tx;
        tx.lockTime = input.lockTime;

        auto blockHash = plan.preBlockHash;
        auto blockHeight = plan.preBlockHeight;

        auto outputTo = prepareOutputWithScript(input.toAddress, plan.amount, input.coinType, blockHash, blockHeight);
        if (!outputTo.has_value()) { 
            return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_address);
        }
        tx.outputs.push_back(outputTo.value());

        if (plan.change > 0) {
            auto outputChange = prepareOutputWithScript(input.changeAddress, plan.change, input.coinType, blockHash, blockHeight);
            if (!outputChange.has_value()) { 
                return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_address);
            }
            tx.outputs.push_back(outputChange.value());
        }

        const auto emptyScript = Bitcoin::Script();
        for (auto& utxo : plan.utxos) {
            tx.inputs.emplace_back(utxo.outPoint, emptyScript, utxo.outPoint.sequence);
        }

        // Optional OP_RETURN output
        if (!plan.outputOpReturn.empty()) {
            auto lockingScriptOpReturn = Bitcoin::Script::buildOpReturnScript(plan.outputOpReturn);
            if (lockingScriptOpReturn.bytes.empty()) {
                return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_memo);
            }

            auto emplace_at = tx.outputs.end();
            if (plan.outputOpReturnIndex.has_value()) {
                emplace_at = tx.outputs.begin();
                std::advance(emplace_at, plan.outputOpReturnIndex.value());
            }
            const int64_t amount = 0;
            tx.outputs.emplace(emplace_at, amount, lockingScriptOpReturn);
        }

        // extra outputs (always in the end of the outputs list)
        for (auto& o : input.extraOutputs) {
            auto output = prepareOutputWithScript(o.first, o.second, input.coinType, blockHash, blockHeight);
            if (!output.has_value()) { 
                return Result<Transaction, Common::Proto::SigningError>::failure(Common::Proto::Error_invalid_address);
            }
            tx.outputs.push_back(output.value());
        }

        return Result<Transaction, Common::Proto::SigningError>(tx);
    }

    /// Prepares a TransactionOutput with given address and amount, prepares script for it
    static std::optional<Bitcoin::TransactionOutput> prepareOutputWithScript(const std::string& addr, Bitcoin::Amount amount, enum TWCoinType coin, const Data& blockHash, int64_t blockHeight) {
        auto lockingScript = Bitcoin::Script::lockScriptForAddress(addr, coin, blockHash, blockHeight);
        if (lockingScript.empty()) {
            return {};
        }
        return Bitcoin::TransactionOutput(amount, lockingScript);
    }

};

} // namespace TW::Zen
