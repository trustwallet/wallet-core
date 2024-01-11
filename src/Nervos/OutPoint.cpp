// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "OutPoint.h"
#include "Serialization.h"

namespace TW::Nervos {

void OutPoint::encode(Data& data) const {
    data.insert(data.end(), txHash.begin(), txHash.end());
    encode32LE(index, data);
}

nlohmann::json OutPoint::json() const {
    return nlohmann::json{{"tx_hash", hexEncoded(txHash)},
                          {"index", Serialization::numberToHex(uint64_t(index))}};
}

} // namespace TW::Nervos