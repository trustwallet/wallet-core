// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableOp.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

bool sortUTXOIDs(TransferableOp::UTXOID lhs, TransferableOp::UTXOID rhs) {
    // prefer to sort by value of .first
    for (auto i = 0; i < lhs.first.size(); ++i) {
        if (lhs.first[i] != rhs.first[i]) {
            return lhs.first[i] < rhs.first[i];
        }
    }
    // using value of .second to break ties
    return lhs.second < rhs.second;
}


void TransferableOp::encode(Data& data) const {
    for (auto byte : AssetID) {
        data.push_back(byte);
    }
    encode32LE(UTXOIDs.size(), data);
    std::sort(UTXOIDs.begin(), UTXOIDs.end(), sortUTXOIDs);
    for (auto utxoID : UTXOIDs) {
        for (auto byte : utxoID.first) {
            data.push_back(byte);
        }
        encode32LE(utxoID.second, data);
    }
    TransferOp.encode(data);
}

void SECP256k1MintOperation::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(AddressIndices.size(), data);
    std::sort(AddressIndices.begin(), AddressIndices.end());
    for (auto index : AddressIndices) {
        encode32LE(index, data);
    }
    MintOutput.encode(data);
    TransferOutput.encode(data);
}

bool NFTMintOperation::compareOutputForSort(NFTMintOperation::Output lhs, NFTMintOperation::Output rhs) {
    if (std::get<0>(lhs) != std::get<0>(rhs)) {
        return std::get<0>(lhs) < std::get<0>(rhs);
    }
    if (std::get<1>(lhs) != std::get<1>(rhs)) {
        return std::get<1>(lhs) < std::get<1>(rhs);
    }
    if (std::get<2>(lhs) != std::get<2>(rhs)) {
        for (auto i = 0; i < std::get<2>(lhs).size(); ++i) {
            if (std::get<2>(lhs)[i] != std::get<2>(rhs)[i]) {
                return std::get<2>(lhs)[i] < std::get<2>(rhs)[i];
            }
        }
    }
    return false;
}

void NFTMintOperation::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(AddressIndices.size(), data);
    std::sort(AddressIndices.begin(), AddressIndices.end());
    for (auto index : AddressIndices) {
        encode32LE(index, data);
    }
    encode32LE(GroupID, data);
    encode32LE(Payload.size(), data);
    for (auto byte : Payload) {
        data.push_back(byte);
    }
    // go through each output and sort its list of addresses 
    // then, sort the list of outputs 
    for (auto &output : Outputs) {
        std::sort(std::get<2>(output).begin(), std::get<2>(output).begin());
    }
    std::sort(Outputs.begin(), Outputs.end(), compareOutputForSort);
    // end sort 
    encode32LE(Outputs.size(), data);
    for (auto output : Outputs) {
        encode64LE(std::get<0>(output), data);
        encode32LE(std::get<1>(output), data);
        std::vector<Address> addrs = std::get<2>(output);
        encode32LE(addrs.size(), data);
        for (auto addr : addrs) {
            for (auto byte : addr.bytes) {
                data.push_back(byte);
            }
        }
    }
}


void NFTTransferOperation::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(AddressIndices.size(), data);
    std::sort(AddressIndices.begin(), AddressIndices.end());
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
    std::sort(TransferOutput.Addresses.begin(), TransferOutput.Addresses.end(), CompareAddressForSort);
    for (auto Address : TransferOutput.Addresses) {
        for (auto byte : Address.bytes) {
            data.push_back(byte);
        }
    }
}
