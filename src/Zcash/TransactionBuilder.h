// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../proto/Bitcoin.pb.h"
#include  "../HexCoding.h"
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
    static Transaction build(const Bitcoin::TransactionPlan& plan, const std::string& toAddress,
                             const std::string& changeAddress, enum TWCoinType coin, uint32_t lockTime) {
        coin = TWCoinTypeZcash;
        Transaction tx =
            Bitcoin::TransactionBuilder::build<Transaction>(plan, toAddress, changeAddress, coin, lockTime);
        // if not set, always use latest consensus branch id
        if (plan.branchId.empty()) {
            std::copy(BlossomBranchID.begin(), BlossomBranchID.end(), tx.branchId.begin());
        } else {
            std::copy(plan.branchId.begin(), plan.branchId.end(), tx.branchId.begin());
        }
        return tx;
    }
};

} // namespace TW::Zcash
