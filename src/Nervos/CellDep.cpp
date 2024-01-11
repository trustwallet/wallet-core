// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CellDep.h"

#include "../BinaryCoding.h"

namespace TW::Nervos {

CellDep::CellDep(const Proto::CellDep& cellDep)
    : outPoint(cellDep.out_point()) {
    auto&& depTypeString = cellDep.dep_type();
    for (int i = 0; i < (int)(sizeof(DepTypeString) / sizeof(DepTypeString[0])); i++) {
        if (depTypeString == DepTypeString[i]) {
            depType = (DepType)i;
        }
    }
}

Proto::CellDep CellDep::proto() const {
    auto cellDep = Proto::CellDep();
    *cellDep.mutable_out_point() = outPoint.proto();
    cellDep.set_dep_type(DepTypeString[depType]);
    return cellDep;
}

void CellDep::encode(Data& data) const {
    outPoint.encode(data);
    data.emplace_back(depType);
}

nlohmann::json CellDep::json() const {
    return nlohmann::json{{"out_point", outPoint.json()}, {"dep_type", DepTypeString[depType]}};
}

} // namespace TW::Nervos
