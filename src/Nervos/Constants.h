// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "CellDep.h"
#include "OutPoint.h"
#include "../Data.h"
#include "../HexCoding.h"

#include <string>
#include <unordered_map>

namespace TW::Nervos::Constants {

inline uint64_t gTransactionBaseSize = 72;
inline uint64_t gCellDepSize = 37;
inline uint64_t gHeaderDepSize = 32;
inline uint64_t gSingleInputAndWitnessBaseSize = 44;
inline uint64_t gBlankWitnessBytes = 65;
inline uint64_t gUint32Size = 4;
inline uint64_t gMinCellCapacityForNativeToken = 6100000000;
inline uint64_t gMinCellCapacityForSUDT = 14400000000;

inline Data gHashPersonalization{'c', 'k', 'b', '-', 'd', 'e', 'f', 'a',
                                 'u', 'l', 't', '-', 'h', 'a', 's', 'h'};

inline Data gSecp256k1CodeHash =
    parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8");

inline CellDep gSecp256k1CellDep = CellDep(
    OutPoint(parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"), 0),
    DepType::DepGroup);

inline Data gSUDTCodeHash =
    parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5");

inline CellDep gSUDTCellDep = CellDep(
    OutPoint(parse_hex("c7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5"), 0),
    DepType::Code);

inline std::unordered_map<HashType, std::string> gHashTypeRegistry{
    {HashType::Data0, "data"}, {HashType::Type1, "type"}, {HashType::Data1, "data1"}};

inline std::unordered_map<DepType, std::string> gDepTypeRegistry{{DepType::Code, "code"},
                                                                 {DepType::DepGroup, "dep_group"}};

} // namespace TW::Nervos::Constants
