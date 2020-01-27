// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../uint256.h"
#include "../UInt.h"
#include "../Data.h"
#include "../BinaryCoding.h"
#include "ISerializable.h"

namespace TW::NEO {

class CoinReference : public ISerializable {
  public:
    uint256_t prevHash;
    uint16_t prevIndex;

    virtual ~CoinReference() {}

    int64_t size() const override {
        return 32 + 2; // sizeof(prevHash) + sizeof(prevIndex);
    }

    void deserialize(const Data &data, int initial_pos = 0) override {
        prevHash = load<uint256_t>(readBytes(data, 32, initial_pos));
        prevIndex = decode16LE(data.data() + initial_pos + 32);
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