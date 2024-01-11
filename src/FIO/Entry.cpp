// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"
#include "TransactionBuilder.h"

namespace TW::FIO {

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

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data &txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto &input, auto &output) {
            
            Proto::SigningInput in = input;
            auto unsignedTxSerialize = TransactionBuilder::buildUnsignedTxBytes(input); 
            auto preSignData = TransactionBuilder::buildPreSignTxData(TW::data(in.chain_params().chain_id()), unsignedTxSerialize);   
            auto imageHash = Hash::sha256(preSignData);
            output.set_data(preSignData.data(), preSignData.size());
            output.set_data_hash(imageHash.data(), imageHash.size());
       });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, [[maybe_unused]] const auto& publicKey) {
            output = Signer::compile(input, signature);
        });
}


} // namespace TW::FIO
