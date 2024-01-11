// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"
#include "TransactionBuilder.h"
#include "../BinaryCoding.h"

namespace TW::FIO {

void Transaction::set(uint32_t expiryTime, const ChainParams& chainParams) {
    expiration = expiryTime;
    refBlockNumber = static_cast<uint16_t>(chainParams.headBlockNumber & 0xffff);
    refBlockPrefix = static_cast<uint32_t>(chainParams.refBlockPrefix & 0xffffffff);
}

void Transaction::serialize(Data& out) const {
    encode32LE(expiration, out);
    encode16LE(refBlockNumber, out);
    encode32LE(refBlockPrefix, out);
    encode32LE(0, out); // 00000000
    encodeVarInt(actions.size(), out);
    for (const auto& item : actions) {
        item.serialize(out);
    }
}

} // namespace TW::FIO
