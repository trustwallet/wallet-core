// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

/// Normal and special signature modes
enum SigningMode {
    SigningMode_Normal = 0, // normal signing
    SigningMode_HashOnly, // no signing, only collect hash to be signed
    SigningMode_External, // no signing, signatures are provided
};

/// Helper class that performs Decred transaction signing.
class Signer {
  public:
    /// Returns a transaction plan (utxo selection, fee estimation)
    static Bitcoin::Proto::TransactionPlan plan(const Bitcoin::Proto::SigningInput& input) noexcept;

    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Bitcoin::Proto::SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs = {}) noexcept;
  
    /// Collect pre-image hashes to be signed
    static Bitcoin::Proto::PreSigningOutput preImageHashes(const Bitcoin::Proto::SigningInput& input) noexcept;

  private:
    /// Private key and redeem script provider for signing.
    Bitcoin::Proto::SigningInput input;

    /// Determine sign strategy
    SigningMode signingMode = SigningMode_Normal;

    /// For SigningMode_HashOnly, collect hashes (plus corresponding publickey hashes) here
    HashPubkeyList hashesForSigning;

    /// For SigningMode_External, signatures are provided here
    std::optional<SignaturePubkeyList> externalSignatures;

  public:
    /// Transaction plan.
    Bitcoin::TransactionPlan txPlan;

    /// Transaction being signed.
    Transaction _transaction;

  private:
    /// List of signed inputs.
    Bitcoin::TransactionInputs<TransactionInput> signedInputs;

  public:
    /// Initializes a transaction signer.
    Signer() = default;

    /// Initializes a transaction signer with signing input.
    explicit Signer(const Bitcoin::Proto::SigningInput& input,
                    SigningMode mode = SigningMode_Normal,
                    std::optional<SignaturePubkeyList> externalSignatures = {})
        : input(input), signingMode(mode), externalSignatures(externalSignatures) {
        if (input.has_plan()) {
            txPlan = Bitcoin::TransactionPlan(input.plan());
        } else {
            txPlan = TransactionBuilder::plan(input);
        }
        _transaction = TransactionBuilder::build(txPlan, input);
    }

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction, Common::Proto::SigningError> sign();

    /// Signs a particular input.
    ///
    /// \returns the signed transaction script.
    Result<Bitcoin::Script, Common::Proto::SigningError> sign(Bitcoin::Script script, size_t index);

    HashPubkeyList getHashesForSigning() const { return hashesForSigning; }

  private:
    Result<std::vector<Data>, Common::Proto::SigningError> signStep(Bitcoin::Script script, size_t index);
    Data createSignature(const Transaction& transaction, const Bitcoin::Script& script,
                         const Data& key, const Data& publicKeyHash, size_t index);

    /// Returns the private key for the given public key hash.
    Data keyForPublicKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Decred
