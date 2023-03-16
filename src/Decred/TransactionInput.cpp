// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
