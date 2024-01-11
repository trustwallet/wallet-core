// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../proto/Bitcoin.pb.h"
#include  "../HexCoding.h"
#include "../Result.h"
#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>

namespace TW::Zcash {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Bitcoin::TransactionPlan plan(const Bitcoin::SigningInput& input) {
        return Bitcoin::TransactionBuilder::plan(input);
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    template <typename Transaction>
    static Result<Transaction, Common::Proto::SigningError> build(const Bitcoin::TransactionPlan& plan, const Bitcoin::SigningInput& input) {
        auto tx_result =
            Bitcoin::TransactionBuilder::build<Transaction>(plan, input);
        if (!tx_result) { return Result<Transaction, Common::Proto::SigningError>::failure(tx_result.error()); }
        Transaction tx = tx_result.payload();
        // if not set, always use latest consensus branch id
        if (plan.branchId.empty()) {
            std::copy(BlossomBranchID.begin(), BlossomBranchID.end(), tx.branchId.begin());
        } else {
            std::copy(plan.branchId.begin(), plan.branchId.end(), tx.branchId.begin());
        }
        return Result<Transaction, Common::Proto::SigningError>(tx);
    }
};

} // namespace TW::Zcash
