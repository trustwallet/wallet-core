// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../proto/Cosmos.pb.h"
#include "JsonSerialization.h"
#include "ProtobufSerialization.h"

#include "PrivateKey.h"
#include "Data.h"
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input, TWCoinType coin) noexcept {
    switch (input.signing_mode()) {
        case Proto::JSON:
            return signJsonSerialized(input);
        
        case Proto::Protobuf:
        default:
            return signProtobuf(input, coin);
    }
}

Proto::SigningOutput Signer::signJsonSerialized(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto preimage = signaturePreimageJSON(input).dump();
    auto hash = Hash::sha256(preimage);
    auto signedHash = key.sign(hash, TWCurveSECP256k1);

    auto output = Proto::SigningOutput();
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);
    auto txJson = transactionJSON(input, signature);
    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());
    output.set_serialized("");
    output.set_error("");
    return output;
}

Proto::SigningOutput Signer::signProtobuf(const Proto::SigningInput& input, TWCoinType coin) noexcept {
    try {
        const auto serializedTxBody = buildProtoTxBody(input);
        const auto serializedAuthInfo = buildAuthInfo(input, coin);
        const auto signature = buildSignature(input, serializedTxBody, serializedAuthInfo, coin);
        auto serializedTxRaw = buildProtoTxRaw(input, serializedTxBody, serializedAuthInfo, signature);

        auto output = Proto::SigningOutput();
        const string jsonSerialized = buildProtoTxJson(input, serializedTxRaw);
        output.set_serialized(jsonSerialized);
        output.set_signature(signature.data(), signature.size());
        output.set_json("");
        output.set_error("");
        return output;
    } catch (const std::exception& ex) {
        auto output = Proto::SigningOutput();
        output.set_error(std::string("Error: ") + ex.what());
        return output;
    }
}

std::string Signer::signJSON(const std::string& json, const Data& key, TWCoinType coin) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input, coin);
    return output.json();
}
