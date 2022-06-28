// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    Cell() = default;

    // Copy constructor
    Cell(const Cell& cell)
        : outPoint(cell.outPoint)
        , capacity(cell.capacity)
        , lock(cell.lock)
        , type(cell.type)
        , data(cell.data) {}

    // Move constructor
    Cell(Cell&& cell)
        : outPoint(std::move(cell.outPoint))
        , capacity(cell.capacity)
        , lock(std::move(cell.lock))
        , type(std::move(cell.type))
        , data(std::move(cell.data)) {}

    // Copy assignment operator
    Cell& operator=(const Cell& cell) {
        outPoint = cell.outPoint;
        capacity = cell.capacity;
        lock = cell.lock;
        type = cell.type;
        data = cell.data;
        return *this;
    }

    Cell(const Proto::Cell& cell)
        : outPoint(cell.out_point())
        , capacity(cell.capacity())
        , lock(cell.lock())
        , type(cell.type()) {
        auto&& cellData = cell.data();
        data.insert(data.end(), cellData.begin(), cellData.end());
    }

    Proto::Cell proto() const {
        auto cell = Proto::Cell();
        *cell.mutable_out_point() = outPoint.proto();
        cell.set_capacity(capacity);
        *cell.mutable_lock() = lock.proto();
        *cell.mutable_type() = type.proto();
        cell.set_data(std::string(data.begin(), data.end()));
        return cell;
    }
};

/// A list of Cell's
using Cells = std::vector<Cell>;

} // namespace TW::Nervos
