// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OutPoint.h"
#include "../proto/Nervos.pb.h"

namespace TW::Nervos {

enum DepType { DepType_Code = 0, DepType_DepGroup = 1 };

/// Nervos cell dep.
class CellDep {
  public:
    OutPoint outPoint;
    DepType depType;

    /// Initializes a cell dep with a previous output and depType
    CellDep(OutPoint outPoint, DepType depType) : outPoint(std::move(outPoint)), depType(depType) {}

    CellDep(const Proto::CellDep& cellDep) : outPoint(cellDep.out_point()) {
        if (cellDep.dep_type() == "code") {
            depType = DepType_Code;
        } else if (cellDep.dep_type() == "dep_group") {
            depType = DepType_DepGroup;
        }
    }

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    Proto::CellDep proto() const {
        auto cellDep = Proto::CellDep();
        *cellDep.mutable_out_point() = outPoint.proto();
        if (depType == DepType_Code) {
            cellDep.set_dep_type("code");
        } else if (depType == DepType_DepGroup) {
            cellDep.set_dep_type("dep_group");
        }
        return cellDep;
    }
};

/// A list of Cell Deps
class CellDeps : public std::vector<CellDep> {
  public:
    CellDeps() = default;
    CellDeps(const std::vector<CellDep>& vector) : std::vector<CellDep>(vector) {}
    CellDeps(CellDep cellDep) : std::vector<CellDep>({cellDep}) {}
};

} // namespace TW::Nervos
