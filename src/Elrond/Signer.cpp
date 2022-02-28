// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Serialization.h"
#include "TransactionFactory.h"
#include "../PublicKey.h"
#include "HexCoding.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Elrond;
using namespace TW::Elrond::Proto;

SigningOutput Signer::sign(const SigningInput &input) noexcept {
    TransactionFactory factory;

    auto transaction = factory.create(input);
    auto privateKey = PrivateKey(input.private_key());
    auto signableAsString = serializeTransaction(transaction);
    auto signableAsData = TW::data(signableAsString);
    auto signature = privateKey.sign(signableAsData, TWCurveED25519);
    auto encodedSignature = hex(signature);
    auto encoded = serializeSignedTransaction(transaction, encodedSignature);

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_signature(encodedSignature);
    protoOutput.set_encoded(encoded);
    return protoOutput;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = sign(input);
    return output.encoded();
}
