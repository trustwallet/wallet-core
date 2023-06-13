// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "HexCoding.h"
#include "Serialization.h"
#include "TransactionFactory.h"

#include <google/protobuf/util/json_util.h>

namespace TW::MultiversX {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    TransactionFactory factory;

    auto privateKey = PrivateKey(input.private_key());
    auto signableAsData = buildUnsignedTxBytes(input);
    auto signature = privateKey.sign(signableAsData, TWCurveED25519);

    return buildSigningOutput(input, signature);
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = sign(input);
    return output.encoded();
}

Data Signer::buildUnsignedTxBytes(const Proto::SigningInput &input) {
    TransactionFactory factory;
    auto transaction = factory.create(input);
    auto signableAsString = serializeTransaction(transaction);

    auto signableAsData = TW::data(signableAsString);
    return signableAsData;
}

Proto::SigningOutput Signer::buildSigningOutput(const Proto::SigningInput &input, const Data &signature) {
    TransactionFactory factory;

    auto transaction = factory.create(input);
    auto encodedSignature = hex(signature);
    auto encoded = serializeSignedTransaction(transaction, encodedSignature);

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_signature(encodedSignature);
    protoOutput.set_encoded(encoded);
    return protoOutput;
}

} // namespace TW::MultiversX
