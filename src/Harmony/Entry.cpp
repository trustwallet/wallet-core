// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "Address.h"
#include "Signer.h"
#include <proto/TransactionCompiler.pb.h>

using namespace TW;
using namespace std;

namespace TW::Harmony {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address(publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    Address addr;
    if (!Address::decode(address, addr)) {
        return Data();
    }
    return addr.getKeyHash();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON([[maybe_unused]] TWCoinType coin, const std::string& json, const Data& key) const {
    return Signer::signJSON(json, key);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data &txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [=](const auto &input, auto &output) {
            Signer signer(uint256_t(load(input.chain_id())));
            auto unsignedTxBytes = signer.buildUnsignedTxBytes(input);
            auto imageHash = Hash::keccak256(unsignedTxBytes);
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            output.set_data_hash(imageHash.data(), imageHash.size()); });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data &txInputData, const std::vector<Data> &signatures,
                    const std::vector<PublicKey> &publicKeys, Data &dataOut) const {
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto &input, auto &output, const auto& signature, [[maybe_unused]] const auto& publicKey) {
            Signer signer(uint256_t(load(input.chain_id())));
            output = signer.buildSigningOutput(input, signature); });
}

} // namespace TW::Harmony
