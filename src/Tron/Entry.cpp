// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include "Address.h"
#include "Serialization.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"

#include <chrono>

using namespace TW::Tron;
using namespace TW;
using namespace std;
using namespace std::chrono;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {

    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto signer = Signer(input);
            auto preImage = signer.signaturePreimage();
            auto preImageHash = Hash::sha256(preImage);
            output.set_datahash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            const auto signer = Signer(input);
            if (signatures.size() != 1) {
                output.set_errorcode(Common::Proto::Error_no_support_n2n);
                output.set_error(
                    Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }
            output = signer.compile(signatures[0]);
        });
}