// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ValueDecoder.h"

namespace TW::Ethereum::ABI {

uint256_t ValueDecoder::decodeUInt256(Data& data) {
    if (data.size() > 32) {
        return load(subData(data, 0, 32));
    }
    return load(data);
}
} // namespace TW::Ethereum::ABI
