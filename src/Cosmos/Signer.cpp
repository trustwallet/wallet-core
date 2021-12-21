// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "ProtobufSerialization.h"
#include "JsonSerialization.h"
#include "../PrivateKey.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

using namespace TW;
using namespace TW::Cosmos;

using json = nlohmann::json;

Signer::Signer(Proto::SigningInput&& input) {
    this->input = input;
}

std::vector<uint8_t> Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    const auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto hash = Hash::sha256(signaturePreimage(publicKey.bytes));
    auto signature = key.sign(hash, TWCurveSECP256k1);
    return std::vector<uint8_t>(signature.begin(), signature.end() - 1);
}

std::string Signer::signaturePreimage(const Data& publicKey) const {
    switch (input.signing_mode()) {
    case Proto::JSON:
        return signaturePreimageJSON(input).dump();

    case Proto::Protobuf:
    default:
        auto pbk = PublicKey(publicKey, TWPublicKeyTypeSECP256k1);
        return signaturePreimageProto(input, pbk);
    }
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();

    auto signature = sign();

    auto key = PrivateKey(input.private_key());
    const auto publicKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto tx = encodeTransaction(signature, publicKey);

    output.set_serialized(tx);
    output.set_signature(signature.data(), signature.size());

    return output;
}

std::string Signer::encodeTransaction(const Data& signature, const PublicKey& publicKey) const {
    switch (input.signing_mode()) {
    case Proto::JSON:
        return buildJsonTxRaw(input, publicKey, signature);

    case Proto::Protobuf:
    default:
        return buildProtoTxRaw(input, publicKey, signature);
    }

}