// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableOutput.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void TransferableOutput::encode(Data& data) const {
    for (auto byte : AssetID) {
        data.push_back(byte);
    }
    Output.encode(data);
}

void SECP256k1TransferOutput::encode(Data& data) const {
    encode32LE(typeID, data);
    encode64LE(Amount, data);
    encode64LE(Locktime, data);
    encode32LE(Threshold, data);
    encode32LE(Addresses.size());
    std::sort(Addresses.begin(), Addresses.end());
    for (auto Address : Addresses) {
        for (auto byte : Address.bytes) {
            data.push_back(byte);
        }
    }
}

void SECP256k1MintOutput::encode(Data& data) const {
    encode32LE(typeID, data);
    encode64LE(Locktime, data);
    encode32LE(Threshold, data);
    encode32LE(Addresses.size());
    std::sort(Addresses.begin(), Addresses.end());
    for (auto Address : Addresses) {
        for (auto byte : Address.bytes) {
            data.push_back(byte);
        }
    }
}

void NFTTransferOutput::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(GroupID, data);
    encode32LE(Payload.size(), data);
    for (auto byte : Payload) {
        data.push_back(byte);
    }
    encode64LE(Locktime, data);
    encode32LE(Threshold, data);
    encode32LE(Addresses.size(), data);
    std::sort(Addresses.begin(), Addresses.end());
    for (auto Address : Addresses) {
        for (auto byte : Address.bytes) {
            data.push_back(byte);
        }
    }
}

void NFTMintOutput::encode(Data& data) const {
    encode32LE(typeID, data);
    encode32LE(GroupID, data);
    encode64LE(Locktime, data);
    encode32LE(Threshold, data);
    encode32LE(Addresses.size());
    std::sort(Addresses.begin(), Addresses.end());
    for (auto Address : Addresses) {
        for (auto byte : Address.bytes) {
            data.push_back(byte);
        }
    }
}