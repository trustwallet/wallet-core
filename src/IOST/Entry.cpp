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
    auto input = Proto::SigningInput();
    auto output = TxCompiler::Proto::PreSigningOutput();
    Data transaction;
    if (!input.ParseFromArray(txInputData.data(), (int)txInputData.size())) {
        output.set_errorcode(Common::Proto::Error_input_parse);
        output.set_error("SigningInput parse failed");
        return TW::data(output.SerializeAsString());
    }
    auto publicKey = data((const byte*)input.public_key().data(), input.public_key().size());
    auto unsignedTx = Signer::buildUnsignedTx(input, publicKey, input.algorithm());
    auto hash = Hash::sha3_256(unsignedTx);
    std::string hashStr(hash.begin(), hash.end());
    output.set_data(unsignedTx);
    output.set_datahash(hashStr);
    return TW::data(output.SerializeAsString());
}

void Entry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                    const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    auto input = Proto::SigningInput();
    if (input.ParseFromArray(txInputData.data(), (int)txInputData.size()) &&
        (signatures.size() == 1)) {
        auto publicKey = data((const byte*)input.public_key().data(), input.public_key().size());
        auto output = Signer::buildSignedTx(input, publicKey, input.algorithm(), signatures[0]);
        auto serializedOut = output.SerializeAsString();
        dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
        return;
    }
}