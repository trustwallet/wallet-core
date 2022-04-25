// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include "Account.h"
#include "Signer.h"
#include "../proto/Common.pb.h"
#include "../proto/TransactionCompiler.pb.h"

using namespace TW::IOST;
using namespace TW;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte,
                            const char*) const {
    return Account::isValid(address);
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    return Account(address).string();
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte,
                            const char*) const {
    return Account::encodePubKey(publicKey);
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto signer = Signer(input);
            auto preImage = signer.signaturePreimage();
            auto preImageHash = Hash::sha3_256(preImage);
            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    dataOut = txCompilerTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, [&](const auto& input, auto& output) {
            const auto signer = Signer(input);
            if (signatures.size() != 1 && publicKeys.size() != 1) {
                output.set_error(Common::Proto::Error_no_support_n2n);
                output.set_error_message(
                    Common::Proto::SigningError_Name(Common::Proto::Error_no_support_n2n));
                return;
            }
            output = signer.compile(signatures[0], publicKeys[0]);
        });
}