// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
