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


int ParamTuple::addParam(std::shared_ptr<ParamBase> param) {
    if (!_isDynamic && param->isDynamic()) {
        _isDynamic = true;
    }
    return _params.addParam(param);
}
