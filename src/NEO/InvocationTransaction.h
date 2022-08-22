// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../Data.h"

namespace TW::NEO {

class InvocationTransaction : public Transaction {
public:
    Data script;
    uint64_t gas = 0;

    InvocationTransaction(TransactionType t = TransactionType::TT_InvocationTransaction,
                          TW::byte ver = 1)
        : Transaction(t, ver) {}

    int deserializeExclusiveData(const Data& data, int initial_pos = 0) override {
        uint32_t readBytes = 0;
        script = readVarBytes(data, initial_pos, &readBytes);
        if (version >= 1) {
            gas = decode64LE(data.data() + initial_pos + readBytes);
            readBytes += sizeof(gas);
        }
        return initial_pos + readBytes;
    }

    Data serializeExclusiveData() const override {
        auto resp = writeVarBytes(script);
        if (version >= 1) {
            encode64LE(gas, resp);
        }
        return resp;
    }

    bool operator==(const InvocationTransaction& other) const {
        return this->script == other.script && this->gas == other.gas &&
               Transaction::operator==(other);
    }
};

} // namespace TW::NEO
