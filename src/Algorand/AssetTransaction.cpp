//
// Created by Andrew Vasiliev on 26.07.2021.
//

#include "AssetTransaction.h"
#include "BinaryCoding.h"

using namespace TW;
using namespace TW::Algorand;

Data AssetTransaction::serialize() const {
    Data data;

    // encode map length
    uint8_t size = 10;
    if (!note.empty()) {
        // note is optional
        size += 1;
    }
    data.push_back(0x80 + size);

    // encode fields one by one (sorted by name)
    encodeString("aamt", data);
    encodeNumber(amount, data);

    encodeString("arcv", data);
    encodeBytes(Data(to.bytes.begin(), to.bytes.end()), data);

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
    encodeBytes(Data(from.bytes.begin(), from.bytes.end()), data);

    encodeString("type", data);
    encodeString(type, data);

    encodeString("xaid", data);
    encodeNumber(assetId, data);

    return data;
}

Data AssetTransaction::serialize(Data& signature) const {
    /* Algorand transaction and signature are encoded with msgpack:
    {
        "sig": <signature bytes>
        "txn": <encoded transaction object>,
    }
    */
    Data data;
    // encode map length
    data.push_back(0x80 + 2);
    // signature
    encodeString("sig", data);
    encodeBytes(signature, data);

    // transaction
    encodeString("txn", data);
    append(data, serialize());
    return data;
}
