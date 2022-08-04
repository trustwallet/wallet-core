// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CellDep.h"
#include "Constants.h"

#include "../BinaryCoding.h"

using namespace TW::Nervos;

CellDep::CellDep(const Proto::CellDep& cellDep) : outPoint(cellDep.out_point()) {
    auto&& depTypeString = cellDep.dep_type();
    auto&& depTypeRegistry = Constants::gDepTypeRegistry;
    std::for_each(depTypeRegistry.begin(), depTypeRegistry.end(),
                  [&](const std::pair<DepType, std::string>& p) {
                      if (p.second == depTypeString) {
                          depType = p.first;
                      }
                  });
}

Proto::CellDep CellDep::proto() const {
    auto cellDep = Proto::CellDep();
    *cellDep.mutable_out_point() = outPoint.proto();
    cellDep.set_dep_type(Constants::gDepTypeRegistry.at(depType));
    return cellDep;
}

void CellDep::encode(Data& data) const {
    outPoint.encode(data);
    data.emplace_back(depType);
}

nlohmann::json CellDep::json() const {
    return nlohmann::json{{"out_point", outPoint.json()}, {"dep_type", Constants::gDepTypeRegistry.at(depType)}};
}
