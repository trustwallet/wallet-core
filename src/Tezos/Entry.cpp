// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "proto/TransactionCompiler.pb.h"
#include "Signer.h"

namespace TW::Tezos {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address(publicKey).string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

std::string Entry::signJSON([[maybe_unused]] TWCoinType coin, const std::string& json, const Data& key) const {
    return Signer::signJSON(json, key);
}

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto operationList = TW::Tezos::OperationList(input.operation_list().branch());
            for (TW::Tezos::Proto::Operation operation : input.operation_list().operations()) {
                operationList.addOperation(operation);
            }

            auto preImage = Signer().buildUnsignedTx(operationList);

            // get preImage hash
            Data watermarkedData = Data();
            watermarkedData.push_back(0x03);
            append(watermarkedData, preImage);
            auto preImageHash = Hash::blake2b(watermarkedData, 32);

            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, [[maybe_unused]] const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1) {
                output.set_error(Common::Proto::Error_signatures_count);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_signatures_count));
                return;
            }

            auto operationList = TW::Tezos::OperationList(input.operation_list().branch());
            for (TW::Tezos::Proto::Operation operation : input.operation_list().operations()) {
                operationList.addOperation(operation);
            }
            auto tx = Signer().buildSignedTx(operationList, signatures[0]);
            
            output.set_encoded(tx.data(), tx.size());
        });
}

} // namespace TW::Tezos
