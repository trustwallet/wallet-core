// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Operation.h"

#include <stdexcept>

using namespace TW::Bitshares;
using json = nlohmann::json;

TransferOperation::TransferOperation(uint64_t senderId, uint64_t recipientId, const Asset &amount,
                                     const Asset &fee, Memo *memo)
    : from({1, 2, senderId}), to({1, 2, recipientId}), memo(memo), amount(amount), fee(fee) {
    validate();
}

void TransferOperation::validate() {
    if (fee.amount < 0) {
        throw std::invalid_argument("Fee cannot be negative.");
    }

    if (from == to) {
        throw std::invalid_argument("Sender and recipient cannot be the same.");
    }

    if (amount.amount <= 0) {
        throw std::invalid_argument("Amount in a transfer operation must be positive.");
    }
}

void TransferOperation::serialize(Data& os) const noexcept {
    Bravo::encodeVarInt64(operationId(), os);
    fee.serialize(os);
    Bravo::encodeVarInt64(from.instance, os);
    Bravo::encodeVarInt64(to.instance, os);
    amount.serialize(os);

    if (memo) {
        os.push_back(1);
        memo->serialize(os);
    } else {
        os.push_back(0);
    }

    os.push_back(0);        // no extensions, so 0
}

json TransferOperation::serialize() const noexcept {
    json obj;
    obj["fee"] = fee.serialize();
    obj["from"] = from.string();
    obj["to"] = to.string();
    obj["amount"] = amount.serialize();

    if (memo) {
        obj["memo"] = memo->serialize();
    }

    obj["extensions"] = json::array();
    return json::array({operationId(), obj});
}
