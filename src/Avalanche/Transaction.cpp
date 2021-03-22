// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;


void BaseTransaction::encode(Data& data) const {
    encode32BE(TypeID, data);
    encode32BE(NetworkID, data);
    for (auto byte : BlockchainID) {
        data.push_back(byte);
    }
    encode32BE(Outputs.size(), data);
    for (auto output : Outputs) {
        output.encode(data);
    }
    encode32BE(Inputs.size(), data);
    for (auto input : Inputs) {
        input.encode(data);
    }
    encode32BE(Memo.size(), data);
    for (auto byte : Memo) {
        data.push_back(byte);
    }
}

void UnsignedCreateAssetTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    encodeString(Name, data);
    encodeString(Symbol, data);
    data.push_back(Denomination);
    encode32BE(InitialStates.size(), data);
    for (auto state : InitialStates) {
        state.encode(data);
    }
}

void UnsignedOperationTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    encode32BE(Operations.size(), data);
    for (auto op : Operations) {
        op.encode(data);
    }
}

void UnsignedImportTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    for (auto byte : SourceChain) {
        data.push_back(byte);
    }
    encode32BE(ImportInputs.size(), data);
    for (auto in : ImportInputs) {
        in.encode(data);
    }
}

void UnsignedExportTransaction::encode(Data& data) const {
    BaseTransaction::encode(data);
    for (auto byte : DestinationChain) {
        data.push_back(byte);
    }
    encode32BE(ExportOutputs.size(), data);
    for (auto out : ExportOutputs) {
        out.encode(data);
    }
}

void SignedTransaction::encode(Data& data) const {
    encode16BE(CodecID, data);
    UnsignedTransaction.encode(data);
    // do not sort Credentials, they will be paired with inputs by index
    encode32BE(Credentials.size(), data);
    for (auto cred : Credentials) {
        cred.encode(data);
    }
}