// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OutPoint.h"

#include "../BinaryCoding.h"

using namespace TW::Nervos;

void OutPoint::encode(Data& data) const {
    data.insert(data.end(), txHash.begin(), txHash.end());
    encode32LE(index, data);
}
