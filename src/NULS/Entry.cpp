// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "Address.h"
#include "Signer.h"

using namespace std;

namespace TW::NULS {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address(publicKey).string();
}

TW::Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto addr = Address(address);
    return {addr.bytes.begin(), addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const TW::Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto signer = Signer(input);
            auto unsignedTxBytes = signer.buildUnsignedTx();
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            Data unsignedTxBytesHash = Hash::sha256(Hash::sha256((unsignedTxBytes)));
            output.set_data_hash(unsignedTxBytesHash.data(), unsignedTxBytesHash.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != publicKeys.size()) {
                output.set_error(Common::Proto::Error_signatures_count);
                output.set_error_message(
                    Common::Proto::SigningError_Name(Common::Proto::Error_signatures_count));
                return;
            }
            const auto signer = Signer(input);
            // verify signatures
            auto unsignedTxBytes = signer.buildUnsignedTx();
            Data unsignedTxBytesHash = Hash::sha256(Hash::sha256((unsignedTxBytes)));
            for (std::vector<Data>::size_type i = 0; i < signatures.size(); i++) {
                if (!publicKeys[i].verify(signatures[i], unsignedTxBytesHash)) {
                    throw std::invalid_argument("invalid signature at " + std::to_string(i));
                }
            }
            std::vector<Data> publicKeysData;
            for (std::vector<PublicKey>::size_type i = 0; i < publicKeys.size(); i++) {
                publicKeysData.push_back(publicKeys[i].bytes);
            }

            auto signedData = signer.buildSignedTx(publicKeysData, signatures, unsignedTxBytes);
            output.set_encoded(signedData.data(), signedData.size());
        });
}

} // namespace TW::NULS
