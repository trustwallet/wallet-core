// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableOp.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

bool TransferableOp::sortUTXOIDs(TransferableOp::UTXOID lhs, TransferableOp::UTXOID rhs) {
    if (lhs.first == rhs.first) {
        return lhs.second < rhs.second;
    }
    return std::lexicographical_compare(lhs.first.begin(), lhs.first.end(), rhs.first.begin(), rhs.first.end());
}

void TransferableOp::encode(Data& data) const {
    for (auto byte : AssetID) {
        data.push_back(byte);
    }
    encode32LE(UTXOIDs.size(), data);
    for (auto utxoID : UTXOIDs) {
        for (auto byte : utxoID.first) {
            data.push_back(byte);
        }
        encode32LE(utxoID.second, data);
    }
    TransferOp.encode(data);
}

bool TransferableOp::operator<(const TransferableOp& other) const {
    Data thisData;
    Data otherData;
    encode(thisData);
    other.encode(otherData);
    return std::lexicographical_compare(thisData.begin(), thisData.end(), otherData.begin(), otherData.end());
}

void SECP256k1MintOperation::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(AddressIndices.size(), data);
    for (auto index : AddressIndices) {
        encode32LE(index, data);
    }
    MintOutput.encode(data);
    TransferOutput.encode(data);
}

void NFTMintOperation::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(AddressIndices.size(), data);
    for (auto index : AddressIndices) {
        encode32LE(index, data);
    }
    encode32LE(GroupID, data);
    encode32LE(Payload.size(), data);
    for (auto byte : Payload) {
        data.push_back(byte);
    }
    EncodeOutputs(Outputs, data);
}


void NFTTransferOperation::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(AddressIndices.size(), data);
    for (auto index : AddressIndices) {
        encode32LE(index, data);
    }
    encode32LE(TransferOutput.GroupID, data);
    encode32LE(TransferOutput.Payload.size(), data);
    for (auto byte : TransferOutput.Payload) {
        data.push_back(byte);
    }
    encode64LE(TransferOutput.Locktime, data);
    encode32LE(TransferOutput.Threshold, data);
    encode32LE(TransferOutput.Addresses.size(), data);
    for (auto Address : TransferOutput.Addresses) {
        for (auto byte : Address.bytes) {
            data.push_back(byte);
        }
    }
}
