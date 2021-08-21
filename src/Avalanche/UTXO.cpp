// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "UTXO.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void UTXO::encode(Data& data) const {
    encode16BE(codecID, data);
    for (auto byte : txID) {
        data.push_back(byte);
    }
    encode32BE(utxoIndex, data);
    for (auto byte : assetID) {
        data.push_back(byte);
    }
    output.encode(data);
}
