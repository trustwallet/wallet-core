// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "XAddress.h"
#include "Signer.h"
#include "../proto/TransactionCompiler.pb.h"

using namespace TW::Ripple;
using namespace std;

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address) || XAddress::isValid(address);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

TW::Data Entry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    return txCompilerTemplate<Proto::SigningInput, TW::TxCompiler::Proto::PreSigningOutput>(
        txInputData, [](const auto& input, auto& output) {
            auto signer = Signer(input);
            auto preimage = signer.preImage();
            output.set_data(preimage.data(), preimage.size());
            auto hash = Hash::sha512(preimage);
            auto preimageHash = Data(hash.begin(), hash.begin() + 32);
            output.set_datahash(preimageHash.data(), preimageHash.size());
        });
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto input = Proto::SigningInput();
    auto output = Proto::SigningOutput();
    if (!input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        output.set_error(TW::Common::Proto::Error_input_parse);
    } else if (signatures.size() != 1 || publicKeys.size() != 1) {
        output.set_error(Common::Proto::Error_no_support_n2n);
    } else {
        auto signer = Signer(input);
        output = signer.compile(signatures[0], publicKeys[0]);
    }
    dataOut = TW::data(output.SerializeAsString());
}