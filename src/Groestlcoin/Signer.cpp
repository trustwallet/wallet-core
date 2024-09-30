// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "Hash.h"
#include "Data.h"
#include "HexCoding.h"
#include "Transaction.h"

namespace TW::Groestlcoin {

using TransactionBuilder = Bitcoin::TransactionBuilder;

TransactionPlan Signer::plan(const SigningInput& input) noexcept {
    auto plan = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::plan(input);
    return plan.proto();
}

SigningOutput Signer::sign(const SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs) noexcept {
    SigningOutput output;
    auto result = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::sign(input, false, optionalExternalSigs);
    if (!result) {
        output.set_error(result.error());
        return output;
    }
    const auto& tx = result.payload();
    *output.mutable_transaction() = tx.proto();

    Data encoded;
    tx.encode(encoded);
    output.set_encoded(encoded.data(), encoded.size());

    Data txHashData = encoded;
    if (tx.hasWitness()) {
        txHashData.clear();
        tx.encode(txHashData, Transaction::SegwitFormatMode::NonSegwit);
    }
    auto txHash = Hash::sha256(txHashData.data(), txHashData.size());
    std::reverse(txHash.begin(), txHash.end());
    output.set_transaction_id(hex(txHash));
    return output;
}

PreSigningOutput Signer::preImageHashes(const SigningInput& input) noexcept {
    PreSigningOutput output;
    auto result = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::preImageHashes(input);
    if (!result) {
        output.set_error(result.error());
        output.set_error_message(Common::Proto::SigningError_Name(result.error()));
        return output;
    }

    auto hashList = result.payload();
    auto* hashPubKeys = output.mutable_hash_public_keys();
    for (auto& h : hashList) {
        auto* hpk = hashPubKeys->Add();
        hpk->set_data_hash(h.first.data(), h.first.size());
        hpk->set_public_key_hash(h.second.data(), h.second.size());
    }
    return output;
}

} // namespace TW::Groestlcoin
