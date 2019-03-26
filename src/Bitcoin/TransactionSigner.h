// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Amount.h"
#include "Script.h"
#include "Transaction.h"
#include "TransactionBuilder.h"
#include "TransactionInput.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../Result.h"
#include "../Zcash/Transaction.h"
#include "../proto/Bitcoin.pb.h"

#include <memory>
#include <string>
#include <vector>

namespace TW {
namespace Bitcoin {

/// Helper class that performs Bitcoin transaction signing.
template <typename Transaction>
class TransactionSigner {
  private:
    /// Private key and redeem script provider for signing.
    Proto::SigningInput input;

  public:
    /// Transaction plan.
    TransactionPlan plan;

    /// Transaction being signed.
    Transaction transaction;

  private:
    /// List of signed inputs.
    std::vector<TransactionInput> signedInputs;

  public:
    /// Initializes a transaction signer with signing input.
    TransactionSigner(Bitcoin::Proto::SigningInput&& input)
        : input(input), plan(TransactionBuilder::plan(input)) {
        transaction = TransactionBuilder::build<Transaction>(plan, input.to_address(),
                                                             input.change_address());
    }

    /// Initializes a transaction signer with signing input, a transaction, and
    /// a hash type.
    TransactionSigner(Bitcoin::Proto::SigningInput&& input, const TransactionPlan& plan)
        : input(input), plan(plan) {
        transaction = TransactionBuilder::build<Transaction>(plan, input.to_address(),
                                                             input.change_address());
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction> sign();

  private:
    Result<void> sign(Script script, size_t index, const Proto::UnspentTransaction& utxo);
    Result<std::vector<Data>> signStep(Script script, size_t index,
                                       const Proto::UnspentTransaction& utxo, uint32_t version);
    Data createSignature(const Transaction& transaction, const Script& script, const Data& key,
                         size_t index, Amount amount, uint32_t version);
    Data pushAll(const std::vector<Data>& results);

    /// Returns the private key for the given public key hash.
    Data keyForPublicKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace Bitcoin
} // namespace TW

/// Wrapper for C interface.
struct TWBitcoinTransactionSigner {
    TW::Bitcoin::TransactionSigner<TW::Bitcoin::Transaction> impl;
};

/// Wrapper for Zcash C interface.
struct TWZcashTransactionSigner {
    TW::Bitcoin::TransactionSigner<TW::Zcash::Transaction> impl;
};
