// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableInput.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void TransferableInput::encode(Data& data) const {
    for (auto byte : txID) {
        data.push_back(byte);
    }
    encode32BE(utxoIndex, data);
    for (auto byte : assetID) {
        data.push_back(byte);
    }
    input->encode(data);
}

bool TransferableInput::operator<(const TransferableInput& other) const {
    if (txID == other.txID) {
        return utxoIndex < other.utxoIndex;
    }
    Data thisTxIDData;
    Data otherTxIDData;
    for (auto byte : txID) {
        thisTxIDData.push_back(byte);
    }
    for (auto byte : other.txID) {
        otherTxIDData.push_back(byte);
    }
    return std::lexicographical_compare(thisTxIDData.begin(), thisTxIDData.end(),
                                        otherTxIDData.begin(), otherTxIDData.end());
}

TransferableInput& TransferableInput::operator=(const TransferableInput& other) {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    }
    // assign members
    txID = other.txID;
    utxoIndex = other.utxoIndex;
    assetID = other.assetID;
    input = other.input->duplicate();
    spendableAddresses = other.spendableAddresses;
    return *this;
}

void SECP256k1TransferInput::encode(Data& data) const {
    encode32BE(typeID, data);
    encode64BE(amount, data);
    encode32BE(static_cast<uint32_t>(addressIndices.size()), data);
    for (auto index : addressIndices) {
        encode32BE(index, data);
    }
}
