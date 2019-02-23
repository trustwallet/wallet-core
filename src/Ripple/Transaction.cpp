// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Ripple;

const int NETWORK_PREFIX = 0x53545800;
const int64_t MAX_ALLOWED_AMOUNT = 100000000000;

Data Transaction::serialize() const {
    auto data = Data();
    /// field must be sorted by field type then by field name
    /// "type"
    encodeType(FieldTypeInt16, 2, data);
    encode16(TransactionTypePayment, data);
    /// "flags"
    encodeType(FieldTypeInt32, 2, data);
    encode32(flags, data);
    /// "sequence"
    encodeType(FieldTypeInt32, 4, data);
    encode32(sequence, data);
    /// "destinationTag"
    if (destination_tag > 0) {
        encodeType(FieldTypeInt32, 14, data);
        encode32(destination_tag, data);
    }
    /// "lastLedgerSequence"
    if (last_ledger_sequence > 0) {
        encodeType(FieldTypeInt32, 27, data);
        encode32(last_ledger_sequence, data);
    }
    /// "amount"
    encodeType(FieldTypeAmount, 1, data);
    append(data, serializeAmount(amount));
    /// "fee"
    encodeType(FieldTypeAmount, 8, data);
    append(data, serializeAmount(fee));
    /// "signingPubKey"
    if (!pub_key.empty()) {
        encodeType(FieldTypeVL, 3, data);
        encodeBytes(pub_key, data);
    }
    /// "txnSignature"
    if (!signature.empty()) {
        encodeType(FieldTypeVL, 4, data);
        encodeBytes(signature, data);
    }
    /// "account"
    encodeType(FieldTypeAccount, 1, data);
    encodeBytes(serializeAddress(account), data);
    /// "destination"
    encodeType(FieldTypeAccount, 3, data);
    encodeBytes(serializeAddress(destination), data);
    return data;
}

Data Transaction::getPreImage() const {
    auto preImage = Data();
    encode32(NETWORK_PREFIX, preImage);
    append(preImage, serialize());
    return preImage;
}

Data Transaction::serializeAmount(int64_t amount) {
    if (amount > MAX_ALLOWED_AMOUNT || amount < 0) {
        return Data();
    }
    auto data = Data();
    encode64(uint64_t(amount), data);
    /// clear first bit to indicate XRP
    data[0] &= 0x7F;
    /// set second bit to indicate positive number
    data[0] |= 0x40;
    return data;
}

Data Transaction::serializeAddress(Address address) {
    auto data = Data(&address.bytes[0] + 1, &address.bytes[21]);
    return data;
}
