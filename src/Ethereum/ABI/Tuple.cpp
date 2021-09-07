// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tuple.h"

#include "Data.h"

#include <string>

using namespace TW;
using namespace TW::Ethereum::ABI;


size_t ParamTuple::getSize() const {
    return _params.getSize();
}

bool ParamTuple::setValueJson(const std::string& value) {
    // TODO
    return false;
}

void ParamTuple::encode(Data& data) const {
    return _params.encode(data);
}

bool ParamTuple::decode(const Data& encoded, size_t& offset_inout) {
    return _params.decode(encoded, offset_inout);
}
