// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "SigningInput.h"
#include "Transaction.h"
#include "TransactionBuilder.h"
#include "../KeyPair.h"
#include "../Result.h"
#include "../proto/Bitcoin.pb.h"

namespace TW::Bitcoin {

/// Frontend class for transaction planning, building, and signing
template <typename Transaction, typename TransactionBuilder>
class TransactionSigner {
public:
    // Create plan for a transaction
    static TransactionPlan plan(const SigningInput& input);

    // Sign an unsigned transaction.  Plan it if needed beforehand.
    static Result<Transaction, Common::Proto::SigningError> sign(const SigningInput& input, bool estimationMode = false);
};

} // namespace TW::Bitcoin
