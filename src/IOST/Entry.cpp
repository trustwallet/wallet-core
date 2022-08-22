// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"
#include "Account.h"
#include "Signer.h"
#include "../Base58.h"
#include "../proto/Common.pb.h"
#include "../proto/TransactionCompiler.pb.h"

using namespace TW;
using namespace std;

namespace TW::IOST {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const string& address, TW::byte, TW::byte,
                            const char*) const {
    return Account::isValid(address);
}

string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, TW::byte,
                            const char*) const {
    return Account::encodePubKey(publicKey);
}

TW::Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& str) const {
    return {Base58::bitcoin.decode(str)};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            const auto signer = Signer(input);
            auto preImage = signer.signaturePreimage();
            auto preImageHash = Hash::sha3_256(preImage);
            output.set_data_hash(preImageHash.data(), preImageHash.size());
            output.set_data(preImage.data(), preImage.size());
        });
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
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
            
            const auto signer = Signer(input);
            output = signer.compile(signatures[0], publicKeys[0]);
        });
}

} // namespace TW::IOST