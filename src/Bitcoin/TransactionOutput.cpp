// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionOutput.h"

#include "../BinaryCoding.h"

namespace TW::Bitcoin {

void TransactionOutput::encode(Data& data) const {
    encode64LE(value, data);
    script.encode(data);
}

} // namespace TW::Bitcoin
