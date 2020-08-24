// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

using namespace TW;
using namespace TW::OasisProtocol;

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

Cbor::Encode Transaction::getCBORmessage() const {
    return Cbor::Encode::array({
        Cbor::Encode::uint(0),
        Cbor::Encode::uint(gasPrice),
        Cbor::Encode::bytes(encodeVaruint(gasAmount)),
        Cbor::Encode::bytes(to.getKeyHash()),
        Cbor::Encode::bytes(encodeVaruint(amount)),
        Cbor::Encode::uint(nonce),
        Cbor::Encode::string(method),
        Cbor::Encode::string(context)
    });
}

Data Transaction::serialize(Data& signature) const {
    auto signedMessage = Cbor::Encode::array({getCBORmessage(), Cbor::Encode::bytes(signature)});
    return signedMessage.encoded();
}
