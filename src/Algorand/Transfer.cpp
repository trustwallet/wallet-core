// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transfer.h"
#include "BinaryCoding.h"

namespace TW::Algorand {

Data Transfer::serialize() const {
    /* Algorand transaction is encoded with msgpack
    {
        amt: 847,
        fee: 488931,
        fv: 51,
        gen: 'mainnet-v1.0',
        gh: <Buffer>
        lv: 61,
        note: <Buffer>
        rcv: <Buffer>
        snd: <Buffer>
        type: 'pay',
    }
    */
    Data data;

    // encode map length
    uint8_t size = 9;
    if (!note.empty()) {
        // note is optional
        size += 1;
    }
    // don't encode 0 amount
    if (amount == 0) {
        size -= 1;
    }
    data.push_back(0x80 + size);

    // encode fields one by one (sorted by name)
    if (amount > 0) {
        encodeString("amt", data);
        encodeNumber(amount, data);
    }

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

    encodeString("rcv", data);
    encodeBytes(Data(to.bytes.begin(), to.bytes.end()), data);

    encodeString("snd", data);
    encodeBytes(Data(from.bytes.begin(), from.bytes.end()), data);

    encodeString("type", data);
    encodeString(type, data);
    return data;
}

} // namespace TW::Algorand
