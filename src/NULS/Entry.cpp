// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include "Address.h"
#include "Signer.h"
#include "../proto/Common.pb.h"
#include <boost/exception/diagnostic_information.hpp>

using namespace TW::NULS;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

TW::Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    const auto addr = Address(address);
    return {addr.bytes.begin(), addr.bytes.end()};
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes(TWCoinType coin, const TW::Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto signer = Signer(input);
            auto unsignedTxBytes = signer.buildUnsignedTx();
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            Data unsignedTxBytesHash = Hash::sha256(Hash::sha256((unsignedTxBytes)));
            output.set_data_hash(unsignedTxBytesHash.data(), unsignedTxBytesHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1 || publicKeys.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }
            auto publicKey = publicKeys[0];
            auto signature = signatures[0];
            const auto signer = Signer(input);
            auto signedData = signer.buildSignedTx(publicKey.bytes, signature);
            output.set_encoded(signedData.data(), signedData.size());
        });
}
