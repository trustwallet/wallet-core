// Copyright © 2017-2022 Trust Wallet.
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

namespace TW::BitcoinDiamond {

struct TransactionBuilder {
    /// Plans a transaction by selecting UTXOs and calculating fees.
    static Bitcoin::TransactionPlan plan(const Bitcoin::SigningInput& input) {
        return Bitcoin::TransactionBuilder::plan(input);
    }

    /// Builds a transaction by selecting UTXOs and calculating fees.
    template <typename Transaction>
    static Transaction build(const Bitcoin::TransactionPlan& plan,
                             const Bitcoin::SigningInput& input) {
        auto tx = Bitcoin::TransactionBuilder::build<Transaction>(plan, input);
        std::copy(plan.preBlockHash.begin(), plan.preBlockHash.end(),
                  std::back_inserter(tx.preBlockHash));
        return tx;
    }
};

} // namespace TW::BitcoinDiamond
