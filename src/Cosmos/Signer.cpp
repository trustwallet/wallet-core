// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "PrivateKey.h"
#include "Serialization.h"

#include "Data.h"
#include "Hash.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto preimage = signaturePreimage(input).dump();
    auto hash = Hash::sha256(preimage);
    auto signedHash = key.sign(hash, TWCurveSECP256k1);

    auto output = Proto::SigningOutput();
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);
    auto txJson = transactionJSON(input, signature);
    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());
    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}
