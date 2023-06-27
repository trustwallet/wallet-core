// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "JsonSerialization.h"
#include "ProtobufSerialization.h"

#include "PrivateKey.h"
#include "Data.h"
#include <google/protobuf/util/json_util.h>

namespace TW::Cosmos {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input, TWCoinType coin) noexcept {
    switch (input.signing_mode()) {
    case Proto::JSON:
        return signJsonSerialized(input, coin);

    case Proto::Protobuf:
    default:
        return signProtobuf(input, coin);
    }
}

std::string Signer::signaturePreimage(const Proto::SigningInput& input, const Data& publicKey, TWCoinType coin) const {
    switch (input.signing_mode()) {
    case Proto::JSON:
        return Json::signaturePreimageJSON(input).dump();

    case Proto::Protobuf:
    default:
        auto pbk = PublicKey(publicKey, TWPublicKeyTypeSECP256k1);
        return Protobuf::signaturePreimageProto(input, pbk, coin);
    }
}

Proto::SigningOutput Signer::signJsonSerialized(const Proto::SigningInput& input, TWCoinType coin) noexcept {
    auto key = PrivateKey(input.private_key());
    auto preimage = Json::signaturePreimageJSON(input).dump();
    auto hash = Hash::sha256(preimage);
    auto signedHash = key.sign(hash, TWCurveSECP256k1);

    auto output = Proto::SigningOutput();
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);
    auto txJson = Json::transactionJSON(input, signature, coin);
    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());
    output.set_serialized("");
    output.set_error_message("");
    output.set_signature_json(txJson["tx"]["signatures"].dump());
    return output;
}

Proto::SigningOutput Signer::signProtobuf(const Proto::SigningInput& input, TWCoinType coin) noexcept {
    using namespace Protobuf;
    using namespace Json;
    try {
        const auto serializedTxBody = buildProtoTxBody(input);
        const auto serializedAuthInfo = buildAuthInfo(input, coin);
        const auto signature = buildSignature(input, serializedTxBody, serializedAuthInfo, coin);
        auto serializedTxRaw = buildProtoTxRaw(serializedTxBody, serializedAuthInfo, signature);

        auto output = Proto::SigningOutput();
        const std::string jsonSerialized = Protobuf::buildProtoTxJson(input, serializedTxRaw);
        auto publicKey = PrivateKey(input.private_key()).getPublicKey(TWPublicKeyTypeSECP256k1);
        auto signatures = nlohmann::json::array({signatureJSON(signature, publicKey.bytes, coin)});
        output.set_serialized(jsonSerialized);
        output.set_signature(signature.data(), signature.size());
        output.set_json("");
        output.set_error_message("");
        output.set_signature_json(signatures.dump());
        return output;
    } catch (const std::exception& ex) {
        auto output = Proto::SigningOutput();
        output.set_error(Common::Proto::Error_internal);
        output.set_error_message(std::string("Error: ") + ex.what());
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

std::string Signer::encodeTransaction(const Proto::SigningInput& input, const Data& signature, const PublicKey& publicKey, TWCoinType coin) const {
    switch (input.signing_mode()) {
    case Proto::JSON:
        return Json::buildJsonTxRaw(input, publicKey, signature, coin);

    case Proto::Protobuf:
    default:
        return Protobuf::buildProtoTxRaw(input, publicKey, signature, coin);
    }
}
} // namespace TW::Cosmos
