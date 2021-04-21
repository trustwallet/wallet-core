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
#include "../Groestlcoin/Transaction.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include "../Result.h"
#include "../Zcash/Transaction.h"
#include "../proto/Bitcoin.pb.h"

#include <memory>
#include <string>
#include <vector>

namespace TW::Bitcoin {

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
    TransactionSigner(Bitcoin::Proto::SigningInput &&input)
        : input(input), plan(TransactionBuilder::plan(input)) {
        std::vector<std::pair<std::string, int64_t>> outputs;
        if (input.extra_outputs_size() == 0) {
          // standard one-destination case, take amount from plan
          outputs.emplace_back(std::pair<std::string, int64_t>{input.to_address(), plan.amount});
        } else {
          // multi-output case
          outputs.emplace_back(std::pair<std::string, int64_t>{input.to_address(), input.amount()});
          for (auto i = 0; i < input.extra_outputs_size(); ++i) {
            outputs.emplace_back(std::pair<std::string, int64_t>(input.extra_outputs(i).to_address(), input.extra_outputs(i).amount()));
          }
        }
        transaction = TransactionBuilder::build<Transaction>(
            plan, outputs, input.change_address(), TWCoinType(input.coin_type()));
    }

    /// Initializes a transaction signer with signing input, a transaction, and
    /// a hash type.
    TransactionSigner(Bitcoin::Proto::SigningInput &&input, const TransactionPlan &plan)
        : input(input), plan(plan) {
        std::vector<std::pair<std::string, int64_t>> outputs;
        if (input.extra_outputs_size() == 0) {
          // standard one-destination case, take amount from plan
          outputs.emplace_back(std::pair<std::string, int64_t>{input.to_address(), plan.amount});
        } else {
          // multi-output case
          outputs.emplace_back(std::pair<std::string, int64_t>{input.to_address(), input.amount()});
          for (auto i = 0; i < input.extra_outputs_size(); ++i) {
            outputs.emplace_back(std::pair<std::string, int64_t>(input.extra_outputs(i).to_address(), input.extra_outputs(i).amount()));
          }
        }
        transaction = TransactionBuilder::build<Transaction>(
            plan, outputs, input.change_address(), TWCoinType(input.coin_type()));
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction> sign();

  private:
    Result<void> sign(Script script, size_t index, const Proto::UnspentTransaction &utxo);
    Result<std::vector<Data>> signStep(Script script, size_t index,
                                       const Proto::UnspentTransaction &utxo, uint32_t version);
    Data createSignature(const Transaction &transaction, const Script &script, const Data &key,
                         size_t index, Amount amount, uint32_t version);
    Data pushAll(const std::vector<Data> &results);

    /// Returns the private key for the given public key hash.
    Data keyForPublicKeyHash(const Data &hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data &hash) const;
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWBitcoinTransactionSigner {
    TW::Bitcoin::TransactionSigner<TW::Bitcoin::Transaction> impl;
};

/// Wrapper for Zcash C interface.
struct TWZcashTransactionSigner {
    TW::Bitcoin::TransactionSigner<TW::Zcash::Transaction> impl;
};

/// Wrapper for Groestlcoin C interface.
struct TWGroestlcoinTransactionSigner {
    TW::Bitcoin::TransactionSigner<TW::Groestlcoin::Transaction> impl;
};
