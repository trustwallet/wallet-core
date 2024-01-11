// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

#include <vector>

namespace TW::Nervos {

struct Witness {
    Data lock;
    Data inputType;
    Data outputType;

    Witness() = default;

    /// Initializes a witness with lock, inputType and outputType.
    Witness(const Data& lock, const Data& inputType, const Data& outputType)
        : lock(lock), inputType(inputType), outputType(outputType) {}

    /// Encodes the witness into the provided buffer.
    void encode(Data& data) const;
};

/// A list of Witness's
using Witnesses = std::vector<Witness>;

} // namespace TW::Nervos
