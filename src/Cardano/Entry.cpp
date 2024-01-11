// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "AddressV3.h"
#include "Signer.h"
#include "../proto/Cardano.pb.h"
#include "../proto/TransactionCompiler.pb.h"

namespace TW::Cardano {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return AddressV3::isValidLegacy(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return AddressV3(publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    return AddressV3(address).data();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            Transaction tx;
            const auto buildRet = Signer::buildTx(tx, input);
            if (buildRet != Common::Proto::OK) {
                output.set_error(buildRet);
                output.set_error_message(Common::Proto::SigningError_Name(buildRet));
                return;
            }
            auto hash = tx.getId();
            auto encoded = tx.encode();
            output.set_data_hash(hash.data(), hash.size());
            output.set_data(encoded.data(), encoded.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            auto encoded = Signer::encodeTransactionWithSig(input, publicKey, signature);
            output.set_encoded(encoded.data(), encoded.size());
            return;
        });
}

} // namespace TW::Cardano
