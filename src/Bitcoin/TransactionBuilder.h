// Copyright © 2017-2020 Trust Wallet.
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

namespace TW::Bitcoin {

class TransactionBuilder {
public:
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static TransactionPlan plan(const Bitcoin::Proto::SigningInput& input);

    /// Builds a transaction by selecting UTXOs and calculating fees.
    template <typename Transaction>
    static Transaction build(const TransactionPlan& plan,
        const std::vector<std::pair<std::string, int64_t>>& outputs,
        const std::string& changeAddress, enum TWCoinType coin)
    {
        Transaction tx;
        for (auto i = 0; i < outputs.size(); ++i) {
            auto lockingScriptTo = Script::lockScriptForAddress(std::get<0>(outputs[i]), coin);
            if (lockingScriptTo.empty()) {
                return {};
            }

            tx.outputs.push_back(TransactionOutput(std::get<1>(outputs[i]), lockingScriptTo));
        }

        if (plan.change > 0) {
            auto lockingScriptChange = Script::lockScriptForAddress(changeAddress, coin);
            tx.outputs.push_back(TransactionOutput(plan.change, lockingScriptChange));
        }

        const auto emptyScript = Script();
        for (auto& utxo : plan.utxos) {
            tx.inputs.emplace_back(utxo.out_point(), emptyScript, utxo.out_point().sequence());
        }

        return tx;
    }

private:
    static int64_t getTotalAmountFromInput(const Bitcoin::Proto::SigningInput& input);
};

} // namespace TW::Bitcoin
