// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include <proto/TransactionCompiler.pb.h>

#include "Ethereum/Address.h"
#include "Signer.h"

namespace TW::VeChain {

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

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto unsignedTxBytes = Signer::buildUnsignedTx(input);
            auto imageHash = Hash::blake2b(unsignedTxBytes, 32);
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            output.set_data_hash(imageHash.data(), imageHash.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, [[maybe_unused]] const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }

            Data signedTx = Signer::buildSignedTx(input, signatures[0]);
            output.set_encoded(signedTx.data(), signedTx.size());
            output.set_signature(signatures[0].data(), signatures[0].size());
        });
}

} // namespace TW::VeChain
