// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"
#include "Account.h"
#include "Signer.h"
#include "../Base58.h"
#include "../proto/Common.pb.h"
#include "../proto/TransactionCompiler.pb.h"

using namespace TW;
using namespace std;

namespace TW::IOST {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const string& address, [[maybe_unused]] const PrefixVariant& addressPrefixp) const {
    return Account::isValid(address);
}

string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Account::encodePubKey(publicKey);
}

TW::Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& str) const {
    return {Base58::decode(str)};
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
    dataOut = txCompilerSingleTemplate<Proto::SigningInput, Proto::SigningOutput>(
        txInputData, signatures, publicKeys,
        [](const auto& input, auto& output, const auto& signature, const auto& publicKey) {
            const auto signer = Signer(input);
            output = signer.compile(signature, publicKey);
        });
}

} // namespace TW::IOST
