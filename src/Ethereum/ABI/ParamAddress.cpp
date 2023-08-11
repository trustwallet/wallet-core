// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamAddress.h"
#include <HexCoding.h>
#include <uint256.h>

namespace TW::Ethereum::ABI {

Data ParamAddress::getData() const {
    Data data = store(_val.getVal(), bytes);
    return data;
}

bool ParamAddress::setValueJson(const std::string& value) {
    _val.setVal(load(parse_hex(value)));
    return true;
}

std::shared_ptr<ParamBase> ParamAddress::clone() const {
    return std::make_shared<ParamAddress>(getData());
}

} // namespace TW::Ethereum::ABI
