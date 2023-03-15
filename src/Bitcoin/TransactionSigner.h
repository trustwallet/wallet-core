// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "SigningInput.h"
#include "Transaction.h"
#include "TransactionBuilder.h"
#include "Signer.h"
#include "Data.h"
#include "../KeyPair.h"
#include "../Result.h"
#include "../proto/Bitcoin.pb.h"
#include "../CoinEntry.h"

#include <vector>
#include <optional>
#include <utility>

namespace TW::Bitcoin {

/// Frontend class for transaction planning, building, and signing
template <typename Transaction, typename TransactionBuilder>
class TransactionSigner {
public:
    // Create plan for a transaction
    static TransactionPlan plan(const SigningInput& input);

    // Sign an unsigned transaction.  Plan it if needed beforehand.
    static Result<Transaction, Common::Proto::SigningError> sign(const SigningInput& input, bool estimationMode = false, std::optional<SignaturePubkeyList> optionalExternalSigs = {});

    /// Collect pre-image hashes to be signed
    static Result<HashPubkeyList, Common::Proto::SigningError> preImageHashes(const SigningInput& input);
};

} // namespace TW::Bitcoin
