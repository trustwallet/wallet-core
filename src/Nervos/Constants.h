// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "CellDep.h"
#include "OutPoint.h"
#include "../Data.h"
#include "../HexCoding.h"

namespace TW::Nervos {

enum HashType { Data0 = 0, Type1 = 1, Data1 = 2 };

class Constants {
public:
    static Data getHashPersonalization() {
        return Data(
            {'c', 'k', 'b', '-', 'd', 'e', 'f', 'a', 'u', 'l', 't', '-', 'h', 'a', 's', 'h'});
    }

    static Data getSecp256k1CodeHash() {
        return parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8");
    }

    static CellDep getSecp256k1CellDep() {
        return CellDep(
            OutPoint(parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"),
                     0),
            DepType_DepGroup);
    }

    static Data getSUDTCodeHash() {
        return parse_hex("5e7a36a77e68eecc013dfa2fe6a23f3b6c344b04005808694ae6dd45eea4cfd5");
    }

    static CellDep getSUDTCellDep() {
        return CellDep(
            OutPoint(parse_hex("c7813f6a415144643970c2e88e0bb6ca6a8edc5dd7c1022746f628284a9936d5"),
                     0),
            DepType_Code);
    }

    static std::string hashTypeToString(const HashType hashType) {
        if (hashType == HashType::Data0) {
            return "data";
        } else if (hashType == HashType::Type1) {
            return "type";
        } else if (hashType == HashType::Data1) {
            return "data1";
        }
        return "";
    }

    static HashType stringToHashType(const std::string& string) {
        if (string == "data") {
            return HashType::Data0;
        } else if (string == "type") {
            return HashType::Type1;
        } else if (string == "data1") {
            return HashType::Data1;
        }
        return HashType::Data0;
    }
};
} // namespace TW::Nervos
