// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include <proto/TransactionCompiler.pb.h>
#include "Address.h"
#include "Signer.h"

using namespace TW;
using namespace TW::Cosmos;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char* hrp) const {
    return Address::isValid(coin, address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char* hrp) const {
    return Address(coin, publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const { 
    return Signer::signJSON(json, key);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto pkVec = Data(input.public_key().begin(), input.public_key().end());
            auto preimage = Signer().signaturePreimage(input, pkVec);
            auto imageHash = Hash::sha256(preimage);
            output.set_data(preimage.data(), preimage.size());
            output.set_data_hash(imageHash.data(), imageHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1 || publicKeys.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }

            auto signedTx = Signer().encodeTransaction(input, signatures[0], publicKeys[0]);
            output.set_serialized(signedTx.data(), signedTx.size());
        });
}

