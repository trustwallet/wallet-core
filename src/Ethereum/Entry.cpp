// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Address.h"
#include "HexCoding.h"
#include "proto/Ethereum.pb.h"

namespace TW::Ethereum {

std::string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    return signJSONHelper<Proto::SigningInput, Proto::SigningOutput>(
        coin,
        json,
        key,
        [](const Proto::SigningOutput& output) { return hex(output.encoded()); }
    );
}

} // namespace TW::Ethereum
