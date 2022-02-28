// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../uint256.h"
#include "../Data.h"
#include "../Hash.h"
#include "../BinaryCoding.h"
#include "ISerializable.h"
#include "Serializable.h"

namespace TW::NEO {

class CoinReference : public Serializable {
  public:
    /// Number of bytes for prevIndex.
    static const size_t prevIndexSize = 2;

    uint256_t prevHash;
    uint16_t prevIndex = 0;

    virtual ~CoinReference() {}

    int64_t size() const override {
        return Hash::sha256Size + prevIndexSize;
    }

    void deserialize(const Data& data, int initial_pos = 0) override {
        prevHash = load(readBytes(data, Hash::sha256Size, initial_pos));
        prevIndex = decode16LE(data.data() + initial_pos + Hash::sha256Size);
    }

    Data serialize() const override {
        auto resp = store(prevHash);
        encode16LE(prevIndex, resp);
        return resp;
    }

    bool operator==(const CoinReference &other) const {
        return this->prevHash == other.prevHash
            && this->prevIndex == other.prevIndex;
    }
};

}