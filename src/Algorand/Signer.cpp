// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Algorand;

const Data TRANSACTION_TAG = {84, 88};
const std::string TRANSACTION_PAY = "pay";

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(pubkey);

    auto note = Data(input.note().begin(), input.note().end());
    auto genesisId = input.genesis_id();
    auto genesisHash = Data(input.genesis_hash().begin(), input.genesis_hash().end());
    if (input.has_transaction_pay()) {
        auto message = input.transaction_pay();
        auto to = Address(message.to_address());

        auto transaction = Transaction(from, to, message.fee(), message.amount(), message.first_round(),
                                   message.last_round(), note, TRANSACTION_PAY, genesisId, genesisHash);
        auto signature = sign(key, transaction);
        auto serialized = transaction.serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
    }
    
    return protoOutput;
}

Data Signer::sign(const PrivateKey &privateKey, Transaction &transaction) noexcept {
    Data data;
    append(data, TRANSACTION_TAG);
    append(data, transaction.serialize());
    auto signature = privateKey.sign(data, TWCurveED25519);
    return Data(signature.begin(), signature.end());
}
