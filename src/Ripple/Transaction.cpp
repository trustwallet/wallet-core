// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "Transaction.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <algorithm>

using namespace TW;
using namespace TW::Ripple;

const int NETWORK_PREFIX = 0x53545800;

Data Transaction::serialize() const {
    auto data = Data();
    /// field must be sorted by field type then by field name
    /// "type"
    encodeType(FieldType::int16, 2, data);
    encode16BE(uint16_t(TransactionType::payment), data);
    /// "flags"
    encodeType(FieldType::int32, 2, data);
    encode32BE(static_cast<uint32_t>(flags), data);
    /// "sequence"
    encodeType(FieldType::int32, 4, data);
    encode32BE(sequence, data);
    /// "destinationTag"
    if (encode_tag) {
        encodeType(FieldType::int32, 14, data);
        encode32BE(static_cast<uint32_t>(destination_tag), data);
    }
    /// "lastLedgerSequence"
    if (last_ledger_sequence > 0) {
        encodeType(FieldType::int32, 27, data);
        encode32BE(last_ledger_sequence, data);
    }
    /// "amount"
    encodeType(FieldType::amount, 1, data);
    append(data, serializeAmount(amount));
    /// "fee"
    encodeType(FieldType::amount, 8, data);
    append(data, serializeAmount(fee));
    /// "signingPubKey"
    if (!pub_key.empty()) {
        encodeType(FieldType::vl, 3, data);
        encodeBytes(pub_key, data);
    }
    /// "txnSignature"
    if (!signature.empty()) {
        encodeType(FieldType::vl, 4, data);
        encodeBytes(signature, data);
    }
    /// "account"
    encodeType(FieldType::account, 1, data);
    encodeBytes(serializeAddress(account), data);
    /// "destination"
    encodeType(FieldType::account, 3, data);
    encodeBytes(destination, data);
    return data;
}

Data Transaction::getPreImage() const {
    auto preImage = Data();
    encode32BE(NETWORK_PREFIX, preImage);
    append(preImage, serialize());
    return preImage;
}

Data Transaction::serializeAmount(int64_t amount) {
    if (amount < 0) {
        return Data();
    }
    auto data = Data();
    encode64BE(uint64_t(amount), data);
    /// clear first bit to indicate XRP
    data[0] &= 0x7F;
    /// set second bit to indicate positive number
    data[0] |= 0x40;
    return data;
}

Data Transaction::serializeAddress(Address address) {
    auto data = Data(20);
    if (!address.bytes.empty()) {
        std::copy(&address.bytes[0] + 1, &address.bytes[0] + std::min(address.bytes.size(), size_t(21)), &data[0]);
    }
    return data;
}
