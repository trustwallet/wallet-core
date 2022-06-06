// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Script.h"
#include "../Data.h"
#include "../proto/Nervos.pb.h"

#include <memory>

namespace TW::Nervos {

/// Nervos cell output.
struct CellOutput {
    int64_t capacity;
    Script lock;
    Script type;

    /// Initializes an empty cell output.
    CellOutput() = default;

    /// Initializes a cell output with a capacity and scripts.
    CellOutput(int64_t capacity, Script lock, Script type)
        : capacity(capacity), lock(lock), type(type) {}

    /// Initializes a CellInput from a Protobuf CellInput.
    CellOutput(const Proto::CellOutput& cellOutput)
        : lock(cellOutput.lock()), type(cellOutput.type()) {
        capacity = cellOutput.capacity();
    }

    /// Encodes the output into the provided buffer.
    void encode(Data& data) const;

    Proto::CellOutput proto() const {
        auto cellOutput = Proto::CellOutput();
        cellOutput.set_capacity(capacity);
        *cellOutput.mutable_lock() = lock.proto();
        *cellOutput.mutable_type() = type.proto();
        return cellOutput;
    }
};

/// A list of Cell Outputs
class CellOutputs : public std::vector<CellOutput> {
  public:
    CellOutputs() = default;
    CellOutputs(const std::vector<CellOutput>& vector) : std::vector<CellOutput>(vector) {}
    CellOutputs(CellOutput cellOutput) : std::vector<CellOutput>({cellOutput}) {}
};

} // namespace TW::Nervos
