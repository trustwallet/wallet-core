// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionInput.h"

#include "../BinaryCoding.h"

using namespace TW::Iocoin;

void TransactionInput::encode(Data& data) const {
    //XXXX auto& outpoint = reinterpret_cast<const TW::Iocoin::OutPoint&>(previousOutput);
    auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(previousOutput);
    outpoint.encode(data);
    script.encode(data);
    encode32LE(sequence, data);
}
