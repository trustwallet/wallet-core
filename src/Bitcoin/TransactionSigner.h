// Copyright © 2017-2020 Trust Wallet.
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
#include "../KeyPair.h"
#include "../Result.h"
#include "../Zcash/Transaction.h"
#include "../Zcash/TransactionBuilder.h"
#include "../proto/Bitcoin.pb.h"

#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace TW::Bitcoin {

/// Helper class that performs Bitcoin transaction signing.
template <typename Transaction, typename TransactionBuilder>
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

    bool estimationMode = false;

  public:
    /// Initializes a transaction signer with signing input.
    /// estimationMode: is set, no real signing is performed, only as much as needed to get the almost-exact signed size 
    TransactionSigner(const Bitcoin::Proto::SigningInput& input, bool estimationMode = false) :
    input(input), estimationMode(estimationMode) {
      if (input.has_plan()) {
        plan = TransactionPlan(input.plan());
      } else {
        plan = TransactionBuilder::plan(input);
      }
      transaction = TransactionBuilder::template build<Transaction>(
        plan, input.to_address(), input.change_address(), TWCoinType(input.coin_type())
      );
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction, Common::Proto::SigningError> sign();

    // helper, return binary encoded transaction (used right after sign())
    static void encodeTx(const Transaction& tx, Data& outData) { tx.encode(outData); }

    // internal, public for testability and Decred
    static Data pushAll(const std::vector<Data>& results);

  private:
    Result<void, Common::Proto::SigningError> sign(Script script, size_t index, const Proto::UnspentTransaction& utxo);
    Result<std::vector<Data>, Common::Proto::SigningError> signStep(Script script, size_t index,
                                       const Proto::UnspentTransaction& utxo, uint32_t version) const;
    Data createSignature(const Transaction& transaction, const Script& script, const std::optional<KeyPair>&,
                         size_t index, Amount amount, uint32_t version) const;

    /// Returns the private key for the given public key hash.
    std::optional<KeyPair> keyPairForPubKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Bitcoin
