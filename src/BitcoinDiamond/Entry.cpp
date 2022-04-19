// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "../Bitcoin/Address.h"
#include "proto/Bitcoin.pb.h"
#include "Signer.h"

using namespace TW::BitcoinDiamond;
using namespace TW;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Bitcoin::Address::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char*) const {
    return Bitcoin::Address(publicKey, p2pkh).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<SigningInput, PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) { 
            output = Signer::preImageHashes(input); 
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto input = SigningInput();
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        HashPubkeyList externalSignatures;
        auto n = std::min(signatures.size(), publicKeys.size());
        for (auto i = 0; i < n; ++i) {
            externalSignatures.push_back(std::make_pair(signatures[i], publicKeys[i].bytes));
        }

        auto serializedOut = Signer::sign(input, externalSignatures).SerializeAsString();
        dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
    }
}
