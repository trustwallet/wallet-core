// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "../proto/EOS.pb.h"
#include <proto/TransactionCompiler.pb.h>

#include "Address.h"
#include "Signer.h"

namespace TW::EOS {

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

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const TW::Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto chainId = Data(input.chain_id().begin(), input.chain_id().end());
            auto unsignedTxBytes = Signer(chainId).buildUnsignedTx(input);
            auto imageHash = Hash::sha256(unsignedTxBytes);
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            output.set_data_hash(imageHash.data(), imageHash.size());
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

            auto chainId = Data(input.chain_id().begin(), input.chain_id().end());
            auto signedTx = Signer(chainId).buildSignedTx(input, signatures[0]);
            output.set_json_encoded(signedTx.data(), signedTx.size());
        });
}

} // namespace TW::EOS
