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
    return _params.addParam(param);
}
