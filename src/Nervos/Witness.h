// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <vector>

namespace TW::Nervos {

class Witness {
public:
    Data lock;
    Data inputType;
    Data outputType;

public:
    Witness() = default;

    /// Encodes the witness into the provided buffer.
    void encode(Data& data) const;
};

/// A list of Witness's
using Witnesses = std::vector<Witness>;

} // namespace TW::Nervos
