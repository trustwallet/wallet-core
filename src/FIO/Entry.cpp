// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"
#include "TransactionBuilder.h"

namespace TW::FIO {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
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
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }

            // support one-to-one transfer now.
            if (signatures.size() != 1 || publicKeys.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(
                    Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }

            output = Signer::compile(input, signatures[0]);
        });
}


} // namespace TW::FIO
