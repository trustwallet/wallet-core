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

class Cell {
public:
    OutPoint outPoint;
    Script lock;
    Script type;
    int64_t capacity;

public:
    Cell() = default;

    // Copy constructor
    Cell(const Cell& cell)
        : outPoint(cell.outPoint), lock(cell.lock), type(cell.type), capacity(cell.capacity) {}

    // Move constructor
    Cell(Cell&& cell)
        : outPoint(std::move(cell.outPoint))
        , lock(std::move(cell.lock))
        , type(std::move(cell.type))
        , capacity(cell.capacity) {}

    // Copy assignment operator
    Cell& operator=(const Cell& cell) {
        outPoint = cell.outPoint;
        lock = cell.lock;
        type = cell.type;
        capacity = cell.capacity;
        return *this;
    }

    Cell(const Proto::Cell& cell)
        : outPoint(cell.out_point())
        , lock(cell.lock())
        , type(cell.type())
        , capacity(cell.capacity()) {}

    Proto::Cell proto() const {
        auto cell = Proto::Cell();
        *cell.mutable_out_point() = outPoint.proto();
        *cell.mutable_lock() = lock.proto();
        *cell.mutable_type() = type.proto();
        cell.set_capacity(capacity);
        return cell;
    }
};

/// A list of Cell's
using Cells = std::vector<Cell>;

} // namespace TW::Nervos
