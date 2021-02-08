// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

using namespace TW;
using namespace TW::Oasis;

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

    // This is the way Oasis indicates sign of the amount
    // https://github.com/oasisprotocol/oasis-core/blob/483bd3a897454e4bc1a8795675e7f29cd4d8e72d/go/common/quantity/quantity.go#L39-L55
    Data small;
    small.assign(data.begin() + i - 1, data.end());
    return small;
}

Cbor::Encode Transaction::encodeMessage() const {

    return Cbor::Encode::map({
        { Cbor::Encode::string("nonce"), Cbor::Encode::uint(nonce) },
        { Cbor::Encode::string("method"), Cbor::Encode::string(method) },
        { Cbor::Encode::string("fee"), Cbor::Encode::map({
                 { Cbor::Encode::string("gas"), Cbor::Encode::uint(gasPrice) },
                 { Cbor::Encode::string("amount"), Cbor::Encode::bytes(encodeVaruint(gasAmount)) }
             })
        },
        { Cbor::Encode::string("body"), Cbor::Encode::map({
                  { Cbor::Encode::string("to"), Cbor::Encode::bytes(to.getKeyHash()) },
                  { Cbor::Encode::string("amount"), Cbor::Encode::bytes(encodeVaruint(amount)) }
              })
        }
    });
}

Data Transaction::serialize(Data& signature, PublicKey& publicKey) const {
    auto signedMessage = Cbor::Encode::map({
            { Cbor::Encode::string("untrusted_raw_value"), Cbor::Encode::bytes(encodeMessage().encoded()) },
            { Cbor::Encode::string("signature"), Cbor::Encode::map({
                   { Cbor::Encode::string("public_key"), Cbor::Encode::bytes(publicKey.bytes) },
                   { Cbor::Encode::string("signature"), Cbor::Encode::bytes(signature) }
                })
            }
        });
    return signedMessage.encoded();
}
