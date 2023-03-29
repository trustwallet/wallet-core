// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamBase.h"

namespace TW::Ethereum::ABI {

// Default implementation for simple types: return encoded value (32 bytes)
Data ParamBase::hashStruct() const {
    Data encoded;
    encode(encoded);
    return encoded;
}

} // namespace TW::Ethereum::ABI
