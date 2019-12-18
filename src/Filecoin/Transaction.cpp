// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

using namespace TW;
using namespace TW::Filecoin;

// encode_varuint encodes a 256-bit number into a big endian encoding, omitting leading zeros.
static Data encode_varuint(const uint256_t& value) {
    Data data;
    encode256BE(data, value, 256);
    int i = 0;
    for (i = 0; i < (int)data.size(); ++i)
        if (data[i] != 0)
            break;
    Data small;
    small.assign(data.begin() + i - 1, data.end());
    return small;
}

Cbor::Encode Transaction::message() const {
    return Cbor::Encode::array(
        {Cbor::Encode::bytes(to.bytes), Cbor::Encode::bytes(from.bytes), Cbor::Encode::uint(nonce),
         Cbor::Encode::bytes(encode_varuint(value)), Cbor::Encode::bytes(encode_varuint(gas_price)),
         Cbor::Encode::bytes(encode_varuint(gas_limit)), Cbor::Encode::uint(0),
         Cbor::Encode::bytes(Data())});
}

Data Transaction::cid() const {
    Data cid = {
        // CIDv1 with CBOR codec
        0x01,
        0x71,
        // Blake2b-256 with 32 byte output
        0xa0,
        0xe4,
        0x02,
        0x20,
    };
    Data hash = Hash::blake2b(message().encoded(), 32);
    cid.insert(cid.end(), hash.begin(), hash.end());
    return cid;
}

Data Transaction::serialize() const {
    Data enc_signature;
    enc_signature.push_back(0x01); // Prepend IKTSecp256k1 type
    enc_signature.insert(enc_signature.end(), signature.begin(), signature.end());
    return Cbor::Encode::array({message(), Cbor::Encode::bytes(enc_signature)}).encoded();
}
