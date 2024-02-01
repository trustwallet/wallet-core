// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "OptInAssetTransaction.h"
#include "BinaryCoding.h"

namespace TW::Algorand {

Data OptInAssetTransaction::serialize() const {
    Data data;

    // encode map length
    uint8_t size = 9;
    if (!note.empty()) {
        // note is optional
        size += 1;
    }
    data.push_back(0x80 + size);

    // encode fields one by one (sorted by name)
    encodeString("arcv", data);
    encodeBytes(Data(address.bytes.begin(), address.bytes.end()), data);

    encodeString("fee", data);
    encodeNumber(fee, data);

    encodeString("fv", data);
    encodeNumber(firstRound, data);

    encodeString("gen", data);
    encodeString(genesisId, data);

    encodeString("gh", data);
    encodeBytes(genesisHash, data);

    encodeString("lv", data);
    encodeNumber(lastRound, data);

    if (!note.empty()) {
        encodeString("note", data);
        encodeBytes(note, data);
    }

    encodeString("snd", data);
    encodeBytes(Data(address.bytes.begin(), address.bytes.end()), data);

    encodeString("type", data);
    encodeString(type, data);

    encodeString("xaid", data);
    encodeNumber(assetId, data);

    return data;
}

} // namespace TW::Algorand
