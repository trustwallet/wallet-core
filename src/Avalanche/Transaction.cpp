// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void BaseTransaction::encode(Data& data) const {
    encode32BE(typeID, data);
    encode32BE(networkID, data);
    for (auto byte : blockchainID) {
        data.push_back(byte);
    }
    encode32BE(static_cast<uint32_t>(outputs.size()), data);
    for (auto output : outputs) {
        output.encode(data);
    }
    encode32BE(static_cast<uint32_t>(inputs.size()), data);
    for (auto input : inputs) {
        input.encode(data);
    }
    encode32BE(static_cast<uint32_t>(memo.size()), data);
    for (auto byte : memo) {
        data.push_back(byte);
    }
}

void UnsignedOperationTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    encode32BE(static_cast<uint32_t>(operations.size()), data);
    for (auto op : operations) {
        op.encode(data);
    }
}

void UnsignedImportTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    for (auto byte : sourceChain) {
        data.push_back(byte);
    }
    encode32BE(static_cast<uint32_t>(importInputs.size()), data);
    for (auto in : importInputs) {
        in.encode(data);
    }
}

void UnsignedExportTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    for (auto byte : destinationChain) {
        data.push_back(byte);
    }
    encode32BE(static_cast<uint32_t>(exportOutputs.size()), data);
    for (auto out : exportOutputs) {
        out.encode(data);
    }
}

void SignedTransaction::encode(Data& data) const {
    encode16BE(codecID, data);
    unsignedTransaction.encode(data);
    // do not sort Credentials, they will be paired with inputs by index
    encode32BE(static_cast<uint32_t>(Credentials.size()), data);
    for (auto cred : Credentials) {
        cred.encode(data);
    }
}
