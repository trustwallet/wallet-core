// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableInput.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void TransferableInput::encode(Data& data) const {
    for (auto byte : TxID) {
        data.push_back(byte);
    }
    encode32LE(UTXOIndex, data);
    for (auto byte : AssetID) {
        data.push_back(byte);
    }
    Input.encode(data);
}

void SECP256k1TransferInput::encode(Data& data) const {
    encode32LE(typeID, data);
    encode64LE(Amount, data);
    sort(AddressIndices.begin(), AddressIndices.end());
    for (auto index: AddressIndices) {
        encode32LE(index, data);
    }
}
