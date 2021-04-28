// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Cosmos/Signer.h"
#include "Data.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::THORChain;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto& inputCosmos = input.input();
    auto outputCosmos = Cosmos::Signer::sign(inputCosmos);
    auto output = Proto::SigningOutput();
    output.set_signature(outputCosmos.signature());
    output.set_json(outputCosmos.json());
    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.mutable_input()->set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}
