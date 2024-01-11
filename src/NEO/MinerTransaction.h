// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "Transaction.h"

namespace TW::NEO {

class MinerTransaction final: public Transaction {
  public:
    uint32_t nonce;

     size_t deserializeExclusiveData(const Data& data, size_t initial_pos) override {
        nonce = decode32LE(data.data() + initial_pos);
        return initial_pos + 4;
    }

    Data serializeExclusiveData() const override {
        auto resp = Data();
        encode32LE(nonce, resp);
        return resp;
    }

    bool operator==(const MinerTransaction &other) const {
        return this->nonce == other.nonce && Transaction::operator==(other);
    }
};

} // namespace TW::NEO
