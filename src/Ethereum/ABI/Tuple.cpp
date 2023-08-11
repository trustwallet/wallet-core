// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tuple.h"

#include <string>

namespace TW::Ethereum::ABI {

int ParamTuple::addParam(std::shared_ptr<ParamBase> param) {
    return _params.addParam(param);
}

std::shared_ptr<ParamBase> ParamTuple::clone() const {
    auto newTuple = std::make_shared<ParamTuple>();
    newTuple->_params = _params.clone();
    return newTuple;
}

} // namespace TW::Ethereum::ABI