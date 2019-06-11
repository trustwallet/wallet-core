// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Identifiers.h"
#include <Base58.h>
#include <Ethereum/RLP.h>
#include <Hash.h>

using namespace TW;
using namespace TW::Aeternity;

/// RLP returns a byte serialized representation
Data Transaction::encode() {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(Identifiers::objectTagSpendTransaction));
    append(encoded, Ethereum::RLP::encode(Identifiers::rlpMessageVersion));
    append(encoded, Ethereum::RLP::encode(buildTag(sender_id)));
    append(encoded, Ethereum::RLP::encode(buildTag(recipient_id)));
    append(encoded, Ethereum::RLP::encode(amount));
    append(encoded, Ethereum::RLP::encode(fee));
    append(encoded, Ethereum::RLP::encode(ttl));
    append(encoded, Ethereum::RLP::encode(nonce));
    append(encoded, Ethereum::RLP::encode(payload));

    const Data &raw = Ethereum::RLP::encodeList(encoded);
    return raw;
}

//// buildIDTag assemble an id() object
//// see https://github.com/aeternity/protocol/blob/epoch-v0.22.0/serializations.md#the-id-type
TW::Data Transaction::buildTag(const std::string &address) {
    auto payload = address.substr(Identifiers::prefixTransaction.size(), address.size());

    auto data = Data();
    append(data, {Identifiers::iDTagAccount});
    append(data, Base58::bitcoin.decodeCheck(payload));

    return data;
}
