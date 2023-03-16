// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Identifiers.h"
#include <Base58.h>
#include <Ethereum/RLP.h>
#include <Hash.h>

namespace TW::Aeternity {

/// RLP returns a byte serialized representation
Data Transaction::encode() {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(Identifiers::objectTagSpendTransaction));
    append(encoded, Ethereum::RLP::encode(Identifiers::rlpMessageVersion));
    append(encoded, Ethereum::RLP::encode(buildTag(sender_id)));
    append(encoded, Ethereum::RLP::encode(buildTag(recipient_id)));
    append(encoded, encodeSafeZero(amount));
    append(encoded, encodeSafeZero(fee));
    append(encoded, encodeSafeZero(ttl));
    append(encoded, encodeSafeZero(nonce));
    append(encoded, Ethereum::RLP::encode(payload));

    const Data& raw = Ethereum::RLP::encodeList(encoded);
    return raw;
}

TW::Data Transaction::buildTag(const std::string& address) {
    auto payload = address.substr(Identifiers::prefixTransaction.size(), address.size());

    auto data = Data();
    append(data, Identifiers::iDTagAccount);
    append(data, Base58::decodeCheck(payload));

    return data;
}

TW::Data Transaction::encodeSafeZero(uint256_t value) {
    if (value == 0) {
        return Ethereum::RLP::encode(Data{0});
    }
    return Ethereum::RLP::encode(value);
}

} // namespace TW::Aeternity
