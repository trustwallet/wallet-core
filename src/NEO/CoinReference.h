// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../uint256.h"
#include "Data.h"
#include "../Hash.h"
#include "../BinaryCoding.h"
#include "ISerializable.h"
#include "Serializable.h"

namespace TW::NEO {

class CoinReference final: public Serializable {
  public:
    /// Number of bytes for prevIndex.
    static const size_t prevIndexSize = 2;
    static const size_t prevHashSize = 32;

    uint256_t prevHash;
    uint16_t prevIndex = 0;

    ~CoinReference() override = default;

    size_t size() const override {
        return Hash::sha256Size + prevIndexSize;
    }

    void deserialize(const Data& data, size_t initial_pos = 0) override {
        if (data.size() < initial_pos + size()) {
            throw std::invalid_argument("Data::Cannot read enough bytes!");
        }
        prevHash = load(readBytes(data, Hash::sha256Size, initial_pos));
        prevIndex = decode16LE(data.data() + initial_pos + Hash::sha256Size);
    }

    Data serialize() const override {
        auto resp = store(prevHash, prevHashSize);
        encode16LE(prevIndex, resp);
        return resp;
    }

    bool operator==(const CoinReference &other) const {
        return this->prevHash == other.prevHash
            && this->prevIndex == other.prevIndex;
    }
};

} // namespace TW::NEO
