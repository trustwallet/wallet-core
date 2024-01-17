// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "Ethereum/Address.h"
#include "Signer.h"
#include "../proto/Theta.pb.h"
#include "../proto/TransactionCompiler.pb.h"

namespace TW::Theta {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Ethereum::Address::isValid(address);
}

std::string Entry::normalizeAddress([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    // normalized with EIP55 checksum
    return Ethereum::Address(address).string();
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
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
            const auto signer = Signer(input);
            auto preImage = signer.signaturePreimage();
            auto preImageHash = Hash::keccak256(preImage);
            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            const auto signer = Signer(input);
            output = signer.compile(signature, publicKey);
        });
}

} // namespace TW::Theta
