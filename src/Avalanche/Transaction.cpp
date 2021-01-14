// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void BaseTransaction::encode(Data& data) const {
    encode32LE(TypeID, data);
    encode32LE(NetworkID, data);
    for (auto byte : BlockchainID) {
        data.push_back(byte);
    }
    std::sort(Outputs.begin(), Outputs.end());
    encode32LE(Outputs.size(), data);
    for (auto output : Outputs) {
        output.encode(data);
    }
    std::sort(Inputs.begin(), Inputs.end());
    encode32LE(Inputs.size(), data);
    for (auto input : Inputs) {
        input.encode(data);
    }
    for (auto byte : Memo) {
        data.push_back(byte);
    }
}