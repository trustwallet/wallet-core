// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../uint256.h"
#include "Constants.h"
#include "Data.h"
#include "../BinaryCoding.h"
#include "ReadData.h"
#include "ISerializable.h"
#include "Serializable.h"

namespace TW::NEO {

class TransactionOutput final: public Serializable {
  public:
    uint256_t assetId;
    uint64_t value = 0;
    uint256_t scriptHash;

    ~TransactionOutput() override = default;

    size_t size() const override {
        return assetIdSize + valueSize + scriptHashSize;
    }

    void deserialize(const Data& data, size_t initial_pos = 0) override {
        if (data.size() < initial_pos + size()) {
            throw std::invalid_argument("Data::Cannot read enough bytes!");
        }

        assetId = load(readBytes(data, assetIdSize, initial_pos));
        value = decode64LE(data.data() + initial_pos + assetIdSize);
        scriptHash = load(readBytes(data, scriptHashSize, initial_pos + assetIdSize + valueSize));
    }

    Data serialize() const override {
        auto resp = store(assetId, assetIdSize);
        encode64LE(value, resp);
        return concat(resp, store(scriptHash, scriptHashSize));
    }

    bool operator==(const TransactionOutput &other) const {
        return this->assetId == other.assetId
            && this->value == other.value
            && this->scriptHash == other.scriptHash;
    }
};

} // namespace TW::NEO
