// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Coin.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"

namespace TW::Polkadot {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    if (auto* prefix = std::get_if<SS58Prefix>(&addressPrefix); prefix) {
        return Address::isValid(address, *prefix);
    }
    const auto ss58Prefix = TW::ss58Prefix(coin);
    return Address::isValid(address, ss58Prefix);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    if (auto* ss58Prefix = std::get_if<SS58Prefix>(&addressPrefix); ss58Prefix) {
        return Address(publicKey, *ss58Prefix).string();
    }
    const auto ss58Prefix = TW::ss58Prefix(coin);
    return Address(publicKey, ss58Prefix).string();
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    const auto ss58Prefix = TW::ss58Prefix(coin);
    const auto addr = Address(address, ss58Prefix);
    return {addr.bytes.begin() + 1, addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {

            auto preImage = Signer::signaturePreImage(input);
            auto preImageHash = Signer::hash(preImage);
            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            auto signedTx = Signer::encodeTransaction(input,  publicKey.bytes, signature);
            output.set_encoded(signedTx.data(), signedTx.size());
    });
}
} // namespace TW::Polkadot
