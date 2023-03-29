// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include <nlohmann/json.hpp>

namespace TW::Nervos {

/// Nervos cell input.
struct CellInput {
    /// Reference to the previous transaction's output.
    OutPoint previousOutput;

    /// Prevents the transaction to be mined before an absolute or relative time.
    uint64_t since;

    /// Initializes a cell input with a previous output and since
    CellInput(OutPoint previousOutput, uint64_t since)
        : previousOutput(std::move(previousOutput)), since(since) {}

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    /// Encodes the output into json format.
    nlohmann::json json() const;
};

/// A list of Cell Inputs
using CellInputs = std::vector<CellInput>;

} // namespace TW::Nervos
