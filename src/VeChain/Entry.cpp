// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include <proto/TransactionCompiler.pb.h>
#include "../proto/Common.pb.h"
#include "../Hash.h"

#include "Ethereum/Address.h"
#include "Signer.h"

using namespace TW::VeChain;
using namespace TW;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Ethereum::Address::isValid(address);
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    // normalized with EIP55 checksum
    return Ethereum::Address(address).string();
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Ethereum::Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto unsignedTxBytes = Signer::buildUnsignedTx(input);
            auto imageHash = Hash::blake2b(unsignedTxBytes, 32);
            output.set_data(unsignedTxBytes.data(), unsignedTxBytes.size());
            output.set_datahash(imageHash.data(), imageHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            if (signatures.size() != 1) {
                output.set_errorcode(Common::Proto::Error_no_support_n2n);
                output.set_error(Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }

            Data signedTx = Signer::buildSignedTx(input, signatures[0]);
            output.set_encoded(signedTx.data(), signedTx.size());
            output.set_signature(signatures[0].data(), signatures[0].size());
        });
}
