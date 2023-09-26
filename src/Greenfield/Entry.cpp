// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Ethereum/Address.h"
#include "Signer.h"

namespace TW::Greenfield {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Ethereum::Address::isValid(address);
}

std::string Entry::normalizeAddress([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    // normalized with EIP55 checksum
    return Ethereum::Address(address).string();
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        return Ethereum::Address(publicKey.extended()).string();
    }
    return Ethereum::Address(publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto addr = Ethereum::Address(address);
    return {addr.bytes.begin(), addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            output = Signer::preImageHashes(input);
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            output = Signer::compile(input,  publicKey, signature);
        });
}

} // namespace TW::Greenfield
