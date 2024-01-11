// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "OutPoint.h"
#include "Script.h"
#include "../proto/Nervos.pb.h"

#include <vector>

namespace TW::Nervos {

struct Cell {
    OutPoint outPoint;
    uint64_t capacity;
    Script lock;
    Script type;
    Data data;
    uint64_t blockNumber;
    Data blockHash;
    uint64_t since;
    Data inputType;
    Data outputType;

    Cell() = default;

    // Copy constructor
    Cell(const Cell& cell)
        : outPoint(cell.outPoint)
        , capacity(cell.capacity)
        , lock(cell.lock)
        , type(cell.type)
        , data(cell.data)
        , blockNumber(cell.blockNumber)
        , blockHash(cell.blockHash)
        , since(cell.since)
        , inputType(cell.inputType)
        , outputType(cell.outputType) {}

    // Move constructor
    Cell(Cell&& cell)
        : outPoint(std::move(cell.outPoint))
        , capacity(cell.capacity)
        , lock(std::move(cell.lock))
        , type(std::move(cell.type))
        , data(std::move(cell.data))
        , blockNumber(cell.blockNumber)
        , blockHash(std::move(cell.blockHash))
        , since(cell.since)
        , inputType(std::move(cell.inputType))
        , outputType(std::move(cell.outputType)) {}

    // Copy assignment operator
    Cell& operator=(const Cell& cell) {
        outPoint = cell.outPoint;
        capacity = cell.capacity;
        lock = cell.lock;
        type = cell.type;
        data = cell.data;
        blockNumber = cell.blockNumber;
        blockHash = cell.blockHash;
        since = cell.since;
        inputType = cell.inputType;
        outputType = cell.outputType;
        return *this;
    }

    Cell(const Proto::Cell& cell)
        : outPoint(cell.out_point())
        , capacity(cell.capacity())
        , lock(cell.lock())
        , type(cell.type())
        , blockNumber(cell.block_number())
        , since(cell.since()) {
        auto&& cellData = cell.data();
        data.insert(data.end(), cellData.begin(), cellData.end());
        auto&& cellBlockHash = cell.block_hash();
        blockHash.insert(blockHash.end(), cellBlockHash.begin(), cellBlockHash.end());
        auto&& cellInputType = cell.input_type();
        inputType.insert(inputType.end(), cellInputType.begin(), cellInputType.end());
        auto&& cellOutputType = cell.output_type();
        outputType.insert(outputType.end(), cellOutputType.begin(), cellOutputType.end());
    }

    Proto::Cell proto() const {
        auto cell = Proto::Cell();
        *cell.mutable_out_point() = outPoint.proto();
        cell.set_capacity(capacity);
        *cell.mutable_lock() = lock.proto();
        *cell.mutable_type() = type.proto();
        cell.set_data(std::string(data.begin(), data.end()));
        cell.set_block_number(blockNumber);
        cell.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
        cell.set_since(since);
        cell.set_input_type(std::string(inputType.begin(), inputType.end()));
        cell.set_output_type(std::string(outputType.begin(), outputType.end()));
        return cell;
    }
};

/// A list of Cell's
using Cells = std::vector<Cell>;

} // namespace TW::Nervos
