// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Hash.h"
#include "HexCoding.h"
#include "Transaction.h"
#include "TransactionBuilder.h"
#include "TransactionSigner.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

#include "proto/Common.pb.h"

namespace TW::Bitcoin {

Proto::TransactionPlan Signer::plan(const Proto::SigningInput& input) noexcept {
    auto plan = TransactionSigner<Transaction, TransactionBuilder>::plan(input);
    return plan.proto();
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs) noexcept {
    Proto::SigningOutput output;
    if (input.is_it_brc_operation()) {
        auto serializedInput = data(input.SerializeAsString());
        Rust::CByteArrayWrapper res = Rust::tw_taproot_build_and_sign_transaction(serializedInput.data(), serializedInput.size());
        output.ParseFromArray(res.data.data(), static_cast<int>(res.data.size()));
        return output;
    }
    auto result = TransactionSigner<Transaction, TransactionBuilder>::sign(input, false, optionalExternalSigs);
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
    auto txHash = Hash::sha256d(txHashData.data(), txHashData.size());
    std::reverse(txHash.begin(), txHash.end());
    output.set_transaction_id(hex(txHash));
    return output;
}

Proto::PreSigningOutput Signer::preImageHashes(const Proto::SigningInput& input) noexcept {
    Proto::PreSigningOutput output;
    auto result = TransactionSigner<Transaction, TransactionBuilder>::preImageHashes(input);
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

} // namespace TW::Bitcoin
