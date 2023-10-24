// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

namespace TW::Aptos {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    return validateAddressRust(coin, address, addressPrefix);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    return deriveAddressRust(coin, publicKey, derivation, addressPrefix);
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            output = Signer::preImageHashes(input);
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            output = Signer::compile(input, signature, publicKey);
        });
}

} // namespace TW::Aptos
