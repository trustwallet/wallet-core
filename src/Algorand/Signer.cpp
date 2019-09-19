// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Algorand;

const Data TRANSACTION_TAG = {0x84, 0x88};
const std::string TRANSACTION_PAY = "pay";

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(pubkey);
    auto to = Address(input.to_address());
    auto note = Data(input.note().begin(), input.note().end());

    auto transaction = Transaction(from, to, input.fee(), input.amount(), input.first_round(),
                                   input.last_round(), note, TRANSACTION_PAY);
    auto signature = sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto serialized = transaction.serialize(signature);
    protoOutput.set_encoded(serialized.data(), serialized.size());
    protoOutput.set_signature(signature.data(), signature.size());

    return protoOutput;
}

Data Signer::sign(const PrivateKey &privateKey, Transaction &transaction) noexcept {
    Data data;
    append(data, TRANSACTION_TAG);
    append(data, transaction.serialize());
    auto signature = privateKey.sign(data, TWCurveSECP256k1);
    return Data(signature.begin(), signature.end());
}
