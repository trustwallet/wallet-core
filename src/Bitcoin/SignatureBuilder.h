// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Script.h"
#include "SigningInput.h"
#include "Transaction.h"
#include "TransactionInput.h"
#include "Signer.h"
#include "../proto/Bitcoin.pb.h"
#include "../KeyPair.h"
#include "../Result.h"
#include "../PublicKey.h"
#include "../CoinEntry.h"

#include <utility>
#include <vector>
#include <optional>
#include <utility>

namespace TW::Bitcoin {

/// Normal and special signature modes
enum SigningMode {
    SigningMode_Normal = 0, // normal signing
    SigningMode_SizeEstimationOnly, // no signing, only estimate size of the signature
    SigningMode_HashOnly, // no signing, only collect hash to be signed
    SigningMode_External, // no signing, signatures are provided
};

/// Class that performs Bitcoin transaction signing.
template <typename Transaction>
class SignatureBuilder {
private:
    /// Private key and redeem script provider for signing.
    SigningInput input;

    /// Transaction plan.
    TransactionPlan plan;

    /// Transaction being signed.
    Transaction _transaction;

    /// Transaction being signed, with list of signed inputs
    Transaction transactionToSign;

    SigningMode signingMode = SigningMode_Normal;

    /// For SigningMode_HashOnly, collect hashes (plus corresponding publickey hashes) here
    HashPubkeyList hashesForSigning;

    /// For SigningMode_External, signatures are provided here
    std::optional<SignaturePubkeyList> externalSignatures;

public:
    /// Initializes a transaction signer with signing input.
    /// estimationMode: is set, no real signing is performed, only as much as needed to get the almost-exact signed size 
    SignatureBuilder(
        SigningInput input,
        TransactionPlan plan,
        Transaction& transaction,
        SigningMode signingMode = SigningMode_Normal,
        std::optional<SignaturePubkeyList> externalSignatures = {}
    )
      : input(std::move(input)), plan(std::move(plan)), _transaction(transaction), signingMode(signingMode), externalSignatures(std::move(externalSignatures)) {}

    /// Signs the transaction.
    ///
    /// \returns the signed transaction or an error.
    Result<Transaction, Common::Proto::SigningError> sign();

public:
    // internal, public for testability and Decred
    static Data pushAll(const std::vector<Data>& results);

    HashPubkeyList getHashesForSigning() const { return hashesForSigning; }

private:
    Result<void, Common::Proto::SigningError> sign(Script script, size_t index, const UTXO& utxo);
    Result<std::vector<Data>, Common::Proto::SigningError> signStep(Script script, size_t index,
                                       const UTXO& utxo, uint32_t version);

    Data createSignature(const Transaction& transaction, const Script& script,
                         const Data& publicKeyHash, const std::optional<KeyPair>& key,
                         size_t index, Amount amount, uint32_t version);

    /// Returns the private key for the given public key hash.
    std::optional<KeyPair> keyPairForPubKeyHash(const Data& hash) const;

    /// Returns the redeem script for the given script hash.
    Data scriptForScriptHash(const Data& hash) const;
};

} // namespace TW::Bitcoin
