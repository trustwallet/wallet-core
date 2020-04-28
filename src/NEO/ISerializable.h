// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../BinaryCoding.h"
#include "ReadData.h"

namespace TW::NEO {

class ISerializable {
  public:
    virtual ~ISerializable() {}
    virtual int64_t size() const = 0;
    virtual Data serialize() const = 0;
    virtual void deserialize(const Data& data, int initial_pos = 0) = 0;
};

} // namespace TW::NEO
