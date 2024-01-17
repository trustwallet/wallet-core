// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "../Data.h"

namespace TW::NEO {

class InvocationTransaction final: public Transaction {
public:
    Data script;
    uint64_t gas = 0;

    explicit InvocationTransaction(TransactionType t = TransactionType::TT_InvocationTransaction, byte ver = 1)
        : Transaction(t, ver) {}

    size_t deserializeExclusiveData(const Data& data, size_t initial_pos) override {
        uint32_t readBytes = 0;
        script = readVarBytes(data, initial_pos, &readBytes);
        if (version >= 1) {
            gas = decode64LE(data.data() + initial_pos + readBytes);
            readBytes += sizeof(gas);
        }
        return initial_pos + static_cast<size_t>(readBytes);
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
