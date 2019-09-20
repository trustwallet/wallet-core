// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Algorand;

// Mainnet genesis id
const std::string GENESIS_ID = "mainnet-v1.0";
// Base64 genesis hash wGHE2Pwdvd7S12BL5FaOP20EGYesN73ktiC1qzkkit8=
const Data GENESIS_HASH = {0xc0, 0x61, 0xc4, 0xd8, 0xfc, 0x1d, 0xbd, 0xde, 0xd2, 0xd7, 0x60,
                           0x4b, 0xe4, 0x56, 0x8e, 0x3f, 0x6d, 0x04, 0x19, 0x87, 0xac, 0x37,
                           0xbd, 0xe4, 0xb6, 0x20, 0xb5, 0xab, 0x39, 0x24, 0x8a, 0xdf};
Data Transaction::serialize() const {
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
    auto data = Data();

    // encode map length
    uint8_t size = 9;
    if (!note.empty()) {
        // note is optional
        size += 1;
    }
    data.push_back(0x80 + size);

    // encode fields one by one (sorted by name)
    encodeString("amt", data);
    encodeNumber(amount, data);

    encodeString("fee", data);
    encodeNumber(fee, data);

    encodeString("fv", data);
    encodeNumber(firstRound, data);

    encodeString("gen", data);
    encodeString(GENESIS_ID, data);

    encodeString("gh", data);
    encodeBytes(GENESIS_HASH, data);

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

Data Transaction::serialize(Data &signature) const {
    /* Algorand transaction and signature are encoded with msgpack:
    {
        "sig": <signature bytes>
        "txn": <encoded transaction object>,
    }
    */
    auto data = Data();
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
