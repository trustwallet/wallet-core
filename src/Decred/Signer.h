// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "TransactionBuilder.h"
#include "TransactionInput.h"
#include "../Bitcoin/Amount.h"
#include "../Bitcoin/Script.h"
#include "../Bitcoin/TransactionPlan.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../Result.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Decred.pb.h"

#include <memory>
#include <string>
#include <vector>

namespace TW::Decred {

/// Helper class that performs Decred transaction signing.
class Signer {
  private:
    /// Private key and redeem script provider for signing.
    Bitcoin::Proto::SigningInput input;

  public:
    /// Transaction plan.
    Bitcoin::TransactionPlan plan;

    /// Transaction being signed.
    Transaction transaction;

  private:
    /// List of signed inputs.
    std::vector<TransactionInput> signedInputs;

  public:
    /// Initializes a transaction signer.
    Signer() = default;

    /// Initializes a transaction signer with signing input.
    explicit Signer(Bitcoin::Proto::SigningInput&& input) : input(input) {}

    /// Initializes a transaction signer with signing input, a transaction, and
    /// a hash type.
    Signer(Bitcoin::Proto::SigningInput&& input, const Bitcoin::TransactionPlan& plan)
        : input(input), plan(plan) {
        transaction = TransactionBuilder::build(plan, input.to_address(), input.change_address());
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction> sign();

    /// Signs a particular input.
    ///
    /// \returns the signed transaction script.
    Result<Bitcoin::Script> sign(Bitcoin::Script script, size_t index);

  private:
    Result<std::vector<Data>> signStep(Bitcoin::Script script, size_t index);
    Data createSignature(const Transaction& transaction, const Bitcoin::Script& script,
                         const Data& key, size_t index);
    Data pushAll(const std::vector<Data>& results);

    /// Returns the private key for the given public key hash.
    Data keyForPublicKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Decred

/// Wrapper for C interface.
struct TWDecredSigner {
    TW::Decred::Signer impl;
};
