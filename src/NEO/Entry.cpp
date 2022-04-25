// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"
#include "proto/TransactionCompiler.pb.h"

using namespace TW::NEO;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const {
    return Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto encoded = Signer::signaturePreimage(input);
            auto hash = TW::Hash::sha256(encoded);
            output.set_data_hash(hash.data(), hash.size());
            output.set_data(encoded.data(), encoded.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() == 0 || publicKeys.size() == 0) {
                output.set_error(Common::Proto::Error_invalid_params);
                output.set_error_message("empty signatures or publickeys");
                return;
            }

            // We only support one-to-one transfer now.
            if (signatures.size() != 1 || publicKeys.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message("signatures and publickeys size can only be one");
                return;
            }

            auto encoded = Signer::encodeTransaction(input, publicKeys[0].bytes, signatures[0]);
            output.set_encoded(encoded.data(), encoded.size());
        });
}