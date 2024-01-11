// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionOutput.h"

#include "../BinaryCoding.h"

namespace TW::Decred {

void TransactionOutput::encode(Data& data) const {
    encode64LE(value, data);
    encode16LE(version, data);
    script.encode(data);
}

} // namespace TW::Decred