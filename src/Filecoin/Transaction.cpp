// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

using namespace TW;
using namespace TW::Filecoin;

// encodeVaruint encodes a 256-bit number into a big endian encoding, omitting leading zeros.
static Data encodeVaruint(const uint256_t& value) {
    Data data;
    encode256BE(data, value, 256);
    size_t i = 0;
    for (i = 0; i < data.size(); ++i) {
        if (data[i] != 0) {
            break;
        }
    }
    Data small;
    small.assign(data.begin() + i - 1, data.end());
    return small;
}

// cidPrefix is the CID + Multihash prefix of transaction CIDs.
const Data cidPrefix = {
    // CIDv1 with CBOR codec
    0x01,
    0x71,
    // Blake2b-256 with 32 byte output
    0xa0,
    0xe4,
    0x02,
    0x20,
};

Cbor::Encode Transaction::message() const {
    return Cbor::Encode::array(
        {Cbor::Encode::bytes(to.bytes), Cbor::Encode::bytes(from.bytes), Cbor::Encode::uint(nonce),
         Cbor::Encode::bytes(encodeVaruint(value)), Cbor::Encode::bytes(encodeVaruint(gasPrice)),
         Cbor::Encode::bytes(encodeVaruint(gasLimit)), Cbor::Encode::uint(0),
         Cbor::Encode::bytes(Data())});
}

Data Transaction::cid() const {
    Data cid;
    cid.reserve(cidPrefix.size() + 32);
    cid.insert(cid.end(), cidPrefix.begin(), cidPrefix.end());
    Data hash = Hash::blake2b(message().encoded(), 32);
    cid.insert(cid.end(), hash.begin(), hash.end());
    return cid;
}

Data Transaction::serialize(Data& signature) const {
    // Wrap signature in object.
    Data sigObject;
    TW::append(sigObject, 0x01); // Prepend IKTSecp256k1 type
    TW::append(sigObject, signature);
    // Create Filecoin SignedMessage
    auto signedMessage = Cbor::Encode::array({message(), Cbor::Encode::bytes(sigObject)});
    return signedMessage.encoded();
}
