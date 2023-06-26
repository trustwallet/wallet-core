// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"
#include "OntTxBuilder.h"
#include "OngTxBuilder.h"
#include "Oep4TxBuilder.h"
#include "../proto/TransactionCompiler.pb.h"

namespace TW::Ontology {

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

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto contract = std::string(input.contract().begin(), input.contract().end());
            Data preImage, preImageHash; 

            if (contract == "ONT") {
                auto tx = OntTxBuilder::buildTransferTx(input);
                preImage = tx.serializeUnsigned();
                preImageHash = tx.txHash();
            } else if (contract == "ONG") {
                auto tx = OngTxBuilder::buildTransferTx(input);
                preImage = tx.serializeUnsigned();
                preImageHash = tx.txHash();
            } else {
                auto tx = Oep4TxBuilder::buildTx(input);
                preImage = tx.serializeUnsigned();
                preImageHash = tx.txHash();
            }

            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }
            if (signatures.size() != publicKeys.size()) {
                output.set_error(Common::Proto::Error_signatures_count);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_signatures_count));
                return;
            }

            auto signedTx = Signer::encodeTransaction(input, signatures, publicKeys);
            output.set_encoded(signedTx.data(), signedTx.size());
    });
}
} // namespace TW::Ontology
