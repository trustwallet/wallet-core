// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../proto/Bitcoin.pb.h"
#include  "../HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>

namespace TW::BitcoinDiamond {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Bitcoin::TransactionPlan plan(const Bitcoin::SigningInput& input) {
        return Bitcoin::TransactionBuilder::plan(input);
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    template <typename Transaction>
    static Result<Transaction, Common::Proto::SigningError> build(const Bitcoin::TransactionPlan& plan,
                             const Bitcoin::SigningInput& input) {
        auto tx_result = Bitcoin::TransactionBuilder::build<Transaction>(plan, input);
        if (!tx_result) { return Result<Transaction, Common::Proto::SigningError>::failure(tx_result.error()); }
        Transaction tx = tx_result.payload();
        std::copy(plan.preBlockHash.begin(), plan.preBlockHash.end(),
                  std::back_inserter(tx.preBlockHash));
        return Result<Transaction, Common::Proto::SigningError>(tx);
    }
};

} // namespace TW::BitcoinDiamond
