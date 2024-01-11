// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Script.h"
#include "Data.h"
#include "../proto/Nervos.pb.h"
#include <nlohmann/json.hpp>

#include <memory>

namespace TW::Nervos {

/// Nervos cell output.
struct CellOutput {
    uint64_t capacity;
    Script lock;
    Script type;

    /// Initializes an empty cell output.
    CellOutput() = default;

    /// Initializes a cell output with a capacity and scripts.
    CellOutput(uint64_t capacity, Script&& lock, Script&& type)
        : capacity(capacity), lock(std::move(lock)), type(std::move(type)) {}

    /// Initializes a CellInput from a Protobuf CellInput.
    CellOutput(const Proto::CellOutput& cellOutput)
        : capacity(cellOutput.capacity()), lock(cellOutput.lock()), type(cellOutput.type()) {}

    /// Encodes the output into the provided buffer.
    void encode(Data& data) const;

    /// Encodes the output into json format.
    nlohmann::json json() const;

    Proto::CellOutput proto() const {
        auto cellOutput = Proto::CellOutput();
        cellOutput.set_capacity(capacity);
        *cellOutput.mutable_lock() = lock.proto();
        *cellOutput.mutable_type() = type.proto();
        return cellOutput;
    }
};

/// A list of Cell Outputs
using CellOutputs = std::vector<CellOutput>;

} // namespace TW::Nervos
