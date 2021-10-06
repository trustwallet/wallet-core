// Copyright © 2017-2020 Trust Wallet.
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
  public:
    /// Returns a transaction plan (utxo selection, fee estimation)
    static Bitcoin::Proto::TransactionPlan plan(const Bitcoin::Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Bitcoin::Proto::SigningInput& input) noexcept;
  private:
    /// Private key and redeem script provider for signing.
    Bitcoin::Proto::SigningInput input;

  public:
    /// Transaction plan.
    Bitcoin::TransactionPlan txPlan;

    /// Transaction being signed.
    Transaction transaction;

  private:
    /// List of signed inputs.
    Bitcoin::TransactionInputs<TransactionInput> signedInputs;

  public:
    /// Initializes a transaction signer.
    Signer() = default;

    /// Initializes a transaction signer with signing input.
    explicit Signer(const Bitcoin::Proto::SigningInput& input) 
      : input(input) {
        if (input.has_plan()) {
          txPlan = Bitcoin::TransactionPlan(input.plan());
        } else {
          txPlan = TransactionBuilder::plan(input);
        }
        transaction = TransactionBuilder::build(txPlan, input.to_address(), input.change_address());
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction, Common::Proto::SigningError> sign();

    /// Signs a particular input.
    ///
    /// \returns the signed transaction script.
    Result<Bitcoin::Script, Common::Proto::SigningError> sign(Bitcoin::Script script, size_t index);

  private:
    Result<std::vector<Data>, Common::Proto::SigningError> signStep(Bitcoin::Script script, size_t index);
    Data createSignature(const Transaction& transaction, const Bitcoin::Script& script,
                         const Data& key, size_t index);

    /// Returns the private key for the given public key hash.
    Data keyForPublicKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Decred
