// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Bitcoin/Amount.h"
#include "Bitcoin/Script.h"
#include "Transaction.h"
#include "TransactionBuilder.h"
#include "TransactionInput.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../Result.h"
#include "../proto/Iocoin.pb.h"

#include <memory>
#include <string>
#include <vector>

namespace TW::Iocoin {

/// Helper class that performs Iocoin transaction signing.
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
    TransactionSigner(Iocoin::Proto::SigningInput&& input)
        : input(input), plan(TransactionBuilder::plan(input)) {
        transaction = TransactionBuilder::build<Transaction>(plan, input.to_address(),
                                                             input.change_address());
	transaction.nTime = input.time();
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction> sign();

  private:
    Result<void> sign(Bitcoin::Script script, size_t index, const Proto::UnspentTransaction& utxo);
    Result<std::vector<Data>> signStep(Bitcoin::Script script, size_t index,
                                       const Proto::UnspentTransaction& utxo);
    Data createSignature(const Transaction& transaction, const Bitcoin::Script& script, const Data& key,
                         size_t index, Bitcoin::Amount amount);
    Data pushAll(const std::vector<Data>& results);

    /// Returns the private key for the given public key hash.
    Data keyForPublicKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Iocoin

/// Wrapper for C interface.
struct TWIocoinSigner {
    TW::Iocoin::TransactionSigner<TW::Iocoin::Transaction> impl;
};
