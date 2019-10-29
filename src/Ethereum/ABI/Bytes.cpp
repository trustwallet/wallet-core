// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bytes.h"

namespace TW::Ethereum {

void encode(const Data& bytes, Data& data) {
    encode(uint256_t(bytes.size()), data);

    const auto count = std::min(std::size_t(32), bytes.size());
    const auto padding = ((count + 31) / 32) * 32 - count;
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

} // namespace TW::Ethereum
