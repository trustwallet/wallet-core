// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    if (input.has_planning_v2()) {
        Proto::TransactionPlan plan;

        // Forward the `Bitcoin.Proto.SigningInput.planning_v2` request to Rust.
        auto planningV2Data = data(input.planning_v2().SerializeAsString());
        Rust::TWDataWrapper planningV2DataPtr(planningV2Data);
        Rust::TWDataWrapper planningOutputV2DataPtr = Rust::tw_any_signer_plan(planningV2DataPtr.get(), input.coin_type());

        auto planningOutputV2Data = planningOutputV2DataPtr.toDataOrDefault();
        BitcoinV2::Proto::TransactionPlan planningOutputV2;
        planningOutputV2.ParseFromArray(planningOutputV2Data.data(), static_cast<int>(planningOutputV2Data.size()));

        // Set `Bitcoin.Proto.TransactionPlan.planning_result_v2`. Remain other fields default.
        *plan.mutable_planning_result_v2() = planningOutputV2;
        return plan;
    }

    auto plan = TransactionSigner<Transaction, TransactionBuilder>::plan(input);
    return plan.proto();
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs) noexcept {
    Proto::SigningOutput output;
    if (input.is_it_brc_operation()) {
        auto serializedInput = data(input.SerializeAsString());
        Rust::CByteArrayWrapper res = Rust::tw_bitcoin_legacy_taproot_build_and_sign_transaction(serializedInput.data(), serializedInput.size());
        output.ParseFromArray(res.data.data(), static_cast<int>(res.data.size()));
        return output;
    } else if (input.has_signing_v2()) {
        // Forward the `Bitcoin.Proto.SigningInput.signing_v2` request to Rust.
        auto signingV2Data = data(input.signing_v2().SerializeAsString());
        Rust::TWDataWrapper signingV2DataPtr(signingV2Data);
        Rust::TWDataWrapper signingOutputV2DataPtr = Rust::tw_any_signer_sign(signingV2DataPtr.get(), input.coin_type());

        auto signingOutputV2Data = signingOutputV2DataPtr.toDataOrDefault();
        BitcoinV2::Proto::SigningOutput signingOutputV2;
        signingOutputV2.ParseFromArray(signingOutputV2Data.data(), static_cast<int>(signingOutputV2Data.size()));

        // Set `Bitcoin.Proto.SigningOutput.signing_result_v2`. Remain other fields default.
        *output.mutable_signing_result_v2() = signingOutputV2;
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
