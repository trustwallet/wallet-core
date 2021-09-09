// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Decoder.h"

#include "BinaryCoding.h"
#include "HexCoding.h"
#include "uint256.h"
#include <iterator>

using json = nlohmann::json;

namespace TW::Avalanche {

std::optional<std::string> decodeUTXO(const Data& data) {
    size_t offset = 0;

    // decode codec id
    if (data.size() < 2) { return {}; }
    auto codecId = decode16BE(data.data() + offset);
    // 0x0000 is the only codec id
    if (codecId != 0) { return {}; }
    offset += 2;

    // decode tx hash
    if (data.size() < offset + 32) { return {}; }
    auto txHash = Data(data.begin() + offset, data.begin() + offset + 32);
    offset += 32;

    // decode tx index
    if (data.size() < offset + 4) { return {}; }
    auto index = decode32BE(data.data() + offset);
    offset += 4;

    // decode asset id
    if (data.size() < offset + 32) { return {}; }
    auto assetId = Data(data.begin() + offset, data.begin() + offset + 32);
    offset += 32;

    // decode utxo type
    if (data.size() < offset + 4) { return {}; }
    auto utxoType = decode32BE(data.data() + offset);
    offset += 4;

    // only support SECP256K1 Transfer Output
    if (utxoType != 7) { return {}; }

    // decode amount
    if (data.size() < offset + 8) { return {}; }
    auto amount = decode64BE(data.data() + offset);
    offset += 8;

    // decode lock time
    if (data.size() < offset + 8) { return {}; }
    auto lockTime = decode64BE(data.data() + offset);
    offset += 8;

    // decode threshold
    if (data.size() < offset + 4) { return {}; }
    auto threshold = decode32BE(data.data() + offset);
    offset += 4;

    // decode addresses
    if (data.size() < offset + 4) { return {}; }
    auto len = decode32BE(data.data() + offset);
    offset += 4;

    auto array = json::array();
    for (auto i = 0; i < len; i++) {
        if (data.size() < offset + 20) { return {}; }
        auto address = Data(data.begin() + offset, data.begin() + offset + 20);
        offset += 20;
        array.push_back(hex(address));
    }

    // build decoded json
    auto decoded = json{
        {"codec", codecId},
        {"hash", hex(txHash)},
        {"index", index},
        {"asset", hex(assetId)},
        {"type", utxoType},
        {"amount", amount},
        {"locktime", lockTime},
        {"threshold", threshold},
        {"addresses", array},
    };
    return decoded.dump();
}

} // namespace TW::Avalanche
