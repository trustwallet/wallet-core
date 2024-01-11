// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../BinaryCoding.h"
#include "ReadData.h"

namespace TW::NEO {

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual size_t size() const = 0;
    virtual Data serialize() const = 0;
    virtual void deserialize(const Data& data, size_t initial_pos) = 0;
};

} // namespace TW::NEO
