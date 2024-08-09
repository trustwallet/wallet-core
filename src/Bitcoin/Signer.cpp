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
    if (input.has_signing_v2()) {
        Proto::TransactionPlan plan;

        // Forward the `Bitcoin.Proto.SigningInput.signing_v2` request to Rust.
        auto signingV2Data = data(input.signing_v2().SerializeAsString());
        Rust::TWDataWrapper signingV2DataPtr(signingV2Data);
        Rust::TWDataWrapper transactionPlanV2DataPtr = Rust::tw_any_signer_plan(signingV2DataPtr.get(), input.coin_type());

        auto transactionPlanV2Data = transactionPlanV2DataPtr.toDataOrDefault();
        BitcoinV2::Proto::TransactionPlan transactionPlanV2;
        transactionPlanV2.ParseFromArray(transactionPlanV2Data.data(), static_cast<int>(transactionPlanV2Data.size()));

        // Set `Bitcoin.Proto.TransactionPlan.planning_result_v2`. Remain other fields default.
        *plan.mutable_planning_result_v2() = transactionPlanV2;
        return plan;
    }

    auto plan = TransactionSigner<Transaction, TransactionBuilder>::plan(input);
    return plan.proto();
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input, std::optional<SignaturePubkeyList> optionalExternalSigs) noexcept {
    Proto::SigningOutput output;
    if (input.has_signing_v2()) {
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

    auto result = TransactionSigner<Transaction, TransactionBuilder>::sign(input, false, std::move(optionalExternalSigs));
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
    if (input.has_signing_v2()) {
        // Forward the `Bitcoin.Proto.SigningInput.signing_v2` request to Rust.
        auto signingV2Data = data(input.signing_v2().SerializeAsString());
        Rust::TWDataWrapper signingV2DataPtr(signingV2Data);
        Rust::TWDataWrapper preOutputV2DataPtr = Rust::tw_transaction_compiler_pre_image_hashes(input.coin_type(), signingV2DataPtr.get());

        auto preOutputV2Data = preOutputV2DataPtr.toDataOrDefault();
        BitcoinV2::Proto::PreSigningOutput preSigningOutputV2;
        preSigningOutputV2.ParseFromArray(preOutputV2Data.data(), static_cast<int>(preOutputV2Data.size()));

        // Set `Bitcoin.Proto.PreSigningOutput.pre_signing_result_v2`. Remain other fields default.
        *output.mutable_pre_signing_result_v2() = preSigningOutputV2;
        return output;
    }

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

Proto::SigningOutput Signer::compile(const Proto::SigningInput& input,
                                     const std::vector<Data>& signatures,
                                     const std::vector<PublicKey>& publicKeys) noexcept {
    Proto::SigningOutput output;
    if (input.has_signing_v2()) {
        Rust::TWDataVectorWrapper signaturesVec = signatures;
        Rust::TWDataVectorWrapper publicKeysVec;
        for (const auto& pubkey : publicKeys) {
            publicKeysVec.push(pubkey.bytes);
        }

        // Forward the `Bitcoin.Proto.SigningInput.signing_v2` request to Rust.
        auto signingV2Data = data(input.signing_v2().SerializeAsString());
        Rust::TWDataWrapper signingV2DataPtr(signingV2Data);
        Rust::TWDataWrapper outputV2DataPtr = Rust::tw_transaction_compiler_compile(
            input.coin_type(), signingV2DataPtr.get(), signaturesVec.get(), publicKeysVec.get());

        auto outputV2Data = outputV2DataPtr.toDataOrDefault();
        BitcoinV2::Proto::SigningOutput outputV2;
        outputV2.ParseFromArray(outputV2Data.data(), static_cast<int>(outputV2Data.size()));

        // Set `Bitcoin.Proto.SigningOutput.signing_result_v2`. Remain other fields default.
        *output.mutable_signing_result_v2() = outputV2;
        return output;
    }

    if (signatures.empty() || publicKeys.empty()) {
        output.set_error(Common::Proto::Error_invalid_params);
        output.set_error_message("empty signatures or publickeys");
        return output;
    }

    if (signatures.size() != publicKeys.size()) {
        output.set_error(Common::Proto::Error_invalid_params);
        output.set_error_message("signatures size and publickeys size not equal");
        return output;
    }

    HashPubkeyList externalSignatures;
    auto insertFunctor = [](auto&& signature, auto&& pubkey) noexcept {
        return std::make_pair(signature, pubkey.bytes);
    };
    transform(begin(signatures), end(signatures), begin(publicKeys),
              back_inserter(externalSignatures), insertFunctor);
    output = Signer::sign(input, externalSignatures);
    return output;
}

} // namespace TW::Bitcoin
