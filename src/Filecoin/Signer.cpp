// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "HexCoding.h"
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Filecoin;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    // Load private key and transaction from Protobuf input.
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    Address from_address(pubkey);
    Address to_address(input.to_address());
    Transaction transaction(
        /* to */ to_address,
        /* from */ from_address,
        /* nonce */ input.nonce(),
        /* value */ load(input.value()),
        /* gasPrice */ load(input.gas_price()),
        /* gasLimit */ input.gas_limit());

    // Sign transaction.
    auto signature = sign(key, transaction);
    auto encoded = transaction.serialize(signature);

    // Return Protobuf output.
    Proto::SigningOutput protoOutput;
    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}

Data Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    Data toSign = Hash::blake2b(transaction.cid(), 32);
    auto signature = privateKey.sign(toSign, TWCurveSECP256k1);
    return Data(signature.begin(), signature.end());
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return hex(output.encoded());
}
