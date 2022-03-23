// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Script.h"
#include "SigningInput.h"
#include "Transaction.h"
#include "TransactionInput.h"
#include "../proto/Bitcoin.pb.h"
#include "../KeyPair.h"
#include "../Result.h"

#include <vector>
#include <optional>

namespace TW::Bitcoin {

/// Class that performs Bitcoin transaction signing.
template <typename Transaction>
class SignatureBuilder {
private:
    /// Private key and redeem script provider for signing.
    SigningInput input;

    /// Transaction plan.
    TransactionPlan plan;

    /// Transaction being signed.
    Transaction transaction;

    /// Transaction being signed, with list of signed inputs
    Transaction transactionToSign;

    bool estimationMode = false;

public:
    /// Initializes a transaction signer with signing input.
    /// estimationMode: is set, no real signing is performed, only as much as needed to get the almost-exact signed size 
    SignatureBuilder(const SigningInput& input, const TransactionPlan& plan, Transaction& transaction, bool estimationMode = false)
      : input(input), plan(plan), transaction(transaction), estimationMode(estimationMode) {}

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction, Common::Proto::SigningError> sign();

public:
    // internal, public for testability and Decred
    static Data pushAll(const std::vector<Data>& results);

private:
    Result<void, Common::Proto::SigningError> sign(Script script, size_t index, const UTXO& utxo);
    Result<std::vector<Data>, Common::Proto::SigningError> signStep(Script script, size_t index,
                                       const UTXO& utxo, uint32_t version) const;
    Data createSignature(const Transaction& transaction, const Script& script, const std::optional<KeyPair>&,
                         size_t index, Amount amount, uint32_t version) const;

    /// Returns the private key for the given public key hash.
    std::optional<KeyPair> keyPairForPubKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Bitcoin
