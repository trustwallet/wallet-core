// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../uint256.h"
#include "../Data.h"
#include "../BinaryCoding.h"
#include "ReadData.h"
#include "ISerializable.h"

namespace TW::NEO {

class TransactionOutput : public ISerializable {
  public:
    uint256_t assetId;
    int64_t value;
    uint256_t scriptHash;

    virtual ~TransactionOutput() {}

    int64_t size() const override {
        return store(assetId).size() + 8 + store(scriptHash).size();
    }

    void deserialize(const Data &data, int initial_pos = 0) override {
        assetId = load(readBytes(data, 32, initial_pos));
        value = decode64LE(data.data() + initial_pos + 32);
        scriptHash = load(readBytes(data, 20, initial_pos + 32 + 8));
    }

    Data serialize() const override {
        auto resp = store(assetId);
        encode64LE(value, resp);
        return concat(resp, store(scriptHash));
    }

    bool operator==(const TransactionOutput &other) const {
        return this->assetId == other.assetId
            && this->value == other.value
            && this->scriptHash == other.scriptHash;
    }
};

}