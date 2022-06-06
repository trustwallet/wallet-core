// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "../proto/Nervos.pb.h"

namespace TW::Nervos {

/// Nervos cell input.
class CellInput {
  public:
    /// Reference to the previous transaction's output.
    OutPoint previousOutput;

    /// Prevents the transaction to be mined before an absolute or relative time.
    uint64_t since;

    /// Initializes a cell input with a previous output and since
    CellInput(OutPoint previousOutput, uint64_t since)
        : previousOutput(std::move(previousOutput)), since(since) {}

    /// Initializes a CellInput from a Protobuf CellInput.
    CellInput(const Proto::CellInput& cellInput) : previousOutput(cellInput.previous_output()) {
        since = cellInput.since();
    }

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    Proto::CellInput proto() const {
        auto cellInput = Proto::CellInput();
        *cellInput.mutable_previous_output() = previousOutput.proto();
        cellInput.set_since(since);
        return cellInput;
    }
};

/// A list of Cell Inputs
class CellInputs : public std::vector<CellInput> {
  public:
    CellInputs() = default;
    CellInputs(const std::vector<CellInput>& vector) : std::vector<CellInput>(vector) {}
    CellInputs(CellInput cellInput) : std::vector<CellInput>({cellInput}) {}
};

} // namespace TW::Nervos
