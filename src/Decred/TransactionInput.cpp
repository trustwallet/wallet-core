// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionInput.h"

#include "../BinaryCoding.h"

namespace TW::Decred {

void TransactionInput::encode(Data& data) const {
    previousOutput.encode(data);
    encode32LE(sequence, data);
}

void TransactionInput::encodeWitness(Data& data) const {
    encode64LE(valueIn, data);
    encode32LE(blockHeight, data);
    encode32LE(blockIndex, data);
    script.encode(data);
}

} // namespace TW::Decred
