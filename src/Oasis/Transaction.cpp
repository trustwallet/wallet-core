// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"

using namespace TW;

namespace TW::Oasis {

// clang-format off

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

static Data serializeTransaction(const Data& signature, const PublicKey& publicKey, const Data& body) {
    auto signedMessage = Cbor::Encode::map({
            { Cbor::Encode::string("untrusted_raw_value"), Cbor::Encode::bytes(body) },
            { Cbor::Encode::string("signature"), Cbor::Encode::map({
                   { Cbor::Encode::string("public_key"), Cbor::Encode::bytes(publicKey.bytes) },
                   { Cbor::Encode::string("signature"), Cbor::Encode::bytes(signature) }
                })
            }
        });
    return signedMessage.encoded();
}

static Data buildSignaturePreimage(std::string context, const Data& encodedMessage) {
    Data dataToHash(context.begin(), context.end());
    dataToHash.insert(dataToHash.end(), encodedMessage.begin(), encodedMessage.end());
    return dataToHash;
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

Data Transaction::serialize(const Data& signature, const PublicKey& publicKey) const {
    return serializeTransaction(signature, publicKey, encodeMessage().encoded());
}           
            
Data Transaction::signaturePreimage() const {
    return buildSignaturePreimage(context, encodeMessage().encoded());
}           
            
Cbor::Encode Escrow::encodeMessage() const {

    return Cbor::Encode::map({
        { Cbor::Encode::string("nonce"), Cbor::Encode::uint(nonce) },
        { Cbor::Encode::string("method"), Cbor::Encode::string(method) },
        { Cbor::Encode::string("fee"), Cbor::Encode::map({
                 { Cbor::Encode::string("gas"), Cbor::Encode::uint(gasPrice) },
                 { Cbor::Encode::string("amount"), Cbor::Encode::bytes(encodeVaruint(gasAmount)) }
             })
        },
        { Cbor::Encode::string("body"), Cbor::Encode::map({
                  { Cbor::Encode::string("account"), Cbor::Encode::bytes(account.getKeyHash()) },
                  { Cbor::Encode::string("amount"), Cbor::Encode::bytes(encodeVaruint(amount)) }
              })
        }
    });
}

Data Escrow::serialize(const Data& signature, const PublicKey& publicKey) const {
    return serializeTransaction(signature, publicKey, encodeMessage().encoded());
}

Data Escrow::signaturePreimage() const {
    return buildSignaturePreimage(context, encodeMessage().encoded());
}

Cbor::Encode ReclaimEscrow::encodeMessage() const {

    return Cbor::Encode::map({
        { Cbor::Encode::string("nonce"), Cbor::Encode::uint(nonce) },
        { Cbor::Encode::string("method"), Cbor::Encode::string(method) },
        { Cbor::Encode::string("fee"), Cbor::Encode::map({
                 { Cbor::Encode::string("gas"), Cbor::Encode::uint(gasPrice) },
                 { Cbor::Encode::string("amount"), Cbor::Encode::bytes(encodeVaruint(gasAmount)) }
             })
        },
        { Cbor::Encode::string("body"), Cbor::Encode::map({
                  { Cbor::Encode::string("account"), Cbor::Encode::bytes(account.getKeyHash()) },
                  { Cbor::Encode::string("shares"), Cbor::Encode::bytes(encodeVaruint(shares)) }
              })
        }
    });
}

Data ReclaimEscrow::serialize(const Data& signature, const PublicKey& publicKey) const {
    return serializeTransaction(signature, publicKey, encodeMessage().encoded());
}

Data ReclaimEscrow::signaturePreimage() const {
    return buildSignaturePreimage(context, encodeMessage().encoded());
}
// clang-format on

} // namespace TW::Oasis
