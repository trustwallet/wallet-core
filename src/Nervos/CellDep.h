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
        setDepTypeFromString(cellDep.dep_type());
    }

    void setDepTypeFromString(const std::string& string) {
        if (string == "code") {
            depType = DepType_Code;
        } else if (string == "dep_group") {
            depType = DepType_DepGroup;
        }
    }

    std::string getDepTypeAsString() const {
        if (depType == DepType_Code) {
            return "code";
        } else if (depType == DepType_DepGroup) {
            return "dep_group";
        }
        return "";
    }

    /// Encodes the transaction into the provided buffer.
    void encode(Data& data) const;

    Proto::CellDep proto() const {
        auto cellDep = Proto::CellDep();
        *cellDep.mutable_out_point() = outPoint.proto();
        cellDep.set_dep_type(getDepTypeAsString());
        return cellDep;
    }
};

/// A list of Cell Deps
using CellDeps = std::vector<CellDep>;

} // namespace TW::Nervos
