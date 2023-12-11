// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "HexCoding.h"
#include "proto/Ethereum.pb.h"

#include <google/protobuf/util/json_util.h>

namespace TW::Ethereum {

// TODO call `signRustJSON` when it's done.
std::string Entry::signJSON([[maybe_unused]] TWCoinType coin, const std::string& json, const Data& key) const {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());

    auto inputData = data(input.SerializeAsString());
    Data dataOut;
    sign(coin, inputData, dataOut);

    if(dataOut.empty()) {
        return {};
    }

    Proto::SigningOutput output;
    output.ParseFromArray(dataOut.data(), static_cast<int>(dataOut.size()));

    return hex(output.encoded());
}

} // namespace TW::Ethereum
