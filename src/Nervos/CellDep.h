// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "../proto/Nervos.pb.h"

namespace TW::Nervos {

enum DepType { Code = 0, DepGroup = 1 };

/// Nervos cell dep.
struct CellDep {
    OutPoint outPoint;
    DepType depType;

    /// Initializes a cell dep with a previous output and depType
    CellDep(OutPoint outPoint, DepType depType) : outPoint(std::move(outPoint)), depType(depType) {}

    CellDep(const Proto::CellDep& cellDep);

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    Proto::CellDep proto() const;
};

/// A list of Cell Deps
using CellDeps = std::vector<CellDep>;

} // namespace TW::Nervos
