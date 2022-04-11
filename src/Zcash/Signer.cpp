// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Bitcoin/TransactionSigner.h"
#include "Hash.h"
#include "HexCoding.h"
#include "Transaction.h"
#include "TransactionBuilder.h"

using namespace TW;
using namespace TW::Zcash;

TransactionPlan Signer::plan(const SigningInput& input) noexcept {
    auto plan = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::plan(input);
    return plan.proto();
}

SigningOutput Signer::sign(const SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs) noexcept {
    SigningOutput output;
    auto result = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::sign(input, false, optionalExternalSigs);
    if (!result) {
        output.set_error(result.error());
    } else {
        const auto& tx = result.payload();
        *output.mutable_transaction() = tx.proto();

        Data encoded;
        tx.encode(encoded);
        output.set_encoded(encoded.data(), encoded.size());

        auto txHash = Hash::sha256d(encoded.data(), encoded.size());
        std::reverse(txHash.begin(), txHash.end());
        output.set_transaction_id(hex(txHash));
    }
    return output;
}

PreSigningOutput Signer::preImageHashes(const SigningInput& input) noexcept {
    PreSigningOutput output;
    auto result = Bitcoin::TransactionSigner<Transaction, TransactionBuilder>::preImageHashes(input);
    if (!result) {
        output.set_errorcode(result.error());
        output.set_error(Common::Proto::SigningError_Name(result.error()));
        return output;
    }

    auto hashList = result.payload();
    auto* hashPubKeys = output.mutable_hashpublickeys();
    for (auto& h : hashList) {
        auto* hpk = hashPubKeys->Add();
        hpk->set_datahash(h.first.data(), h.first.size());
        hpk->set_publickeyhash(h.second.data(), h.second.size());
    }
    return output;
}
