// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
